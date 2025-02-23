#include "xparameters.h"
#include "xiic.h"
#include "xintc.h"
#include "xil_exception.h"
#include "xil_printf.h"
#include "math.h"
#include "fpga.h"

#define IIC_DEVICE_ID			XPAR_IIC_0_DEVICE_ID
#define INTC_DEVICE_ID			XPAR_INTC_0_DEVICE_ID
#define INTC_IIC_INTERRUPT_ID	XPAR_INTC_0_IIC_0_VEC_ID

static void SendHandler  (void *CallbackRef, int ByteCount);
static void RecvHandler  (void *CallbackRef, int ByteCount);
static void StatusHandler(void *CallbackRef, int Status);

XIic Iic;
XIntc InterruptController;

volatile struct {
	int EventStatus;
	int RemainingRecvBytes;
	int EventStatusUpdated;
	int RecvBytesUpdated;
	int RemainingSendBytes;
	int SendBytesUpdated;
} HandlerInfo;


int main(void)
{
	
	const int iic_wait = 5000;

	uint32_t* regptr = (uint32_t *)XPAR_M00_AXI_BASEADDR;
	xil_printf("\n\rFPGA_ID = 0x%08x, FPGA_VERSION = 0x%08x\n\r", regptr[FPGA_ID], regptr[FPGA_VERSION]);

	// setup and start the IIC Controller
	XIic_Config *ConfigPtr;
	ConfigPtr = XIic_LookupConfig(IIC_DEVICE_ID);
	XIic_CfgInitialize    (&Iic, ConfigPtr, ConfigPtr->BaseAddress);
	XIic_SetSendHandler   (&Iic, (void *)&HandlerInfo, SendHandler);
	XIic_SetRecvHandler   (&Iic, (void *)&HandlerInfo, RecvHandler);
	XIic_SetStatusHandler (&Iic, (void *)&HandlerInfo, StatusHandler);

	XIntc_Initialize(&InterruptController, INTC_DEVICE_ID);
	XIntc_Connect   (&InterruptController, INTC_IIC_INTERRUPT_ID, XIic_InterruptHandler, &Iic);
	XIntc_Start     (&InterruptController, XIN_REAL_MODE);
	XIntc_Enable    (&InterruptController, INTC_IIC_INTERRUPT_ID);
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler) XIntc_InterruptHandler, &InterruptController);
	Xil_ExceptionEnable();

	HandlerInfo.EventStatusUpdated = FALSE;
	HandlerInfo.RecvBytesUpdated = FALSE;

	XIic_Start(&Iic);


	int tx_status=-1, rx_status=-1;
	xil_printf("%d\n\r", tx_status+rx_status);
	
	u8 tx_ptr[10], rx_ptr[10];

	// transfer rdac to eeprom
 	XIic_SetAddress(&Iic, XII_ADDR_TO_SEND_TYPE, I2C_DIGIPOT);
	tx_ptr[0] = 0xc0;
	tx_status = XIic_MasterSend(&Iic, tx_ptr, 1); 
	for (int i=0; i<iic_wait; i++);

	// read the POT stored EEPROM value.
	tx_ptr[0] = 0x20;
	tx_status = XIic_MasterSend(&Iic, tx_ptr, 1); for (int i=0; i<iic_wait; i++);
	rx_status = XIic_MasterRecv(&Iic, rx_ptr, 1); for (int i=0; i<iic_wait; i++);
	xil_printf("POT Stored EEPROM value = 0x%02x = %03u\n\r", rx_ptr[0], rx_ptr[0]);

	// read the POT tolerance.
	int16_t dpot_tol;
	tx_ptr[0] = 0x3e;
	tx_status = XIic_MasterSend(&Iic, tx_ptr, 1); for (int i=0; i<iic_wait; i++);
	rx_status = XIic_MasterRecv(&Iic, rx_ptr, 2); for (int i=0; i<iic_wait; i++);
	dpot_tol = +(((rx_ptr[0] & 0x7f)<<8) | (rx_ptr[1]<<0));
	if ((rx_ptr[0] & 0x80) != 0) {
		dpot_tol = -dpot_tol;
	}
	xil_printf("POT Tolerance = 0x%02x%02x = %d.%u%%\n\r", rx_ptr[0], rx_ptr[1], dpot_tol/256, (unsigned int)(1000.0*(dpot_tol&0x00ff)/256.0));



	uint32_t whilecount = 0;
	uint8_t dpot_desired;
    float vmon_3v3, vmon_5v0, vmon_12v, vmon_24v;
    //float tempmon0, tempmon1;
    float r1, r2;
	
	while(1) {


		// read the POT stored EEPROM value.
		XIic_SetAddress(&Iic, XII_ADDR_TO_SEND_TYPE, I2C_DIGIPOT);
		tx_ptr[0] = 0x20;
		tx_status = XIic_MasterSend(&Iic, tx_ptr, 1); for (int i=0; i<iic_wait; i++);
		rx_status = XIic_MasterRecv(&Iic, rx_ptr, 1); for (int i=0; i<iic_wait; i++);
		xil_printf("\n\r%d: POT Stored EEPROM value = 0x%02x\n\r", whilecount, rx_ptr[0]);
		

		// read the POT rdac value.
		tx_ptr[0] = 0x00;
		tx_status = XIic_MasterSend(&Iic, tx_ptr, 1); for (int i=0; i<iic_wait; i++);
		rx_status = XIic_MasterRecv(&Iic, rx_ptr, 1); for (int i=0; i<iic_wait; i++);
		xil_printf("POT rdac value = 0x%02x\n\r", rx_ptr[0]);


		// change dpot value if commanded
		if ((regptr[DPOT_CONTROL] & 0x0100) != 0) { // is there a request to change the DPOT setting?
			
			dpot_desired = regptr[DPOT_CONTROL] & 0x00ff;
			xil_printf("\n\r****\n\r**** Programming Digital Potentiometer with 0x%02x\n\r****\n\r", dpot_desired);
			
			// write rdac value
			tx_ptr[0] = 0x00; tx_ptr[1] = dpot_desired;
			tx_status = XIic_MasterSend(&Iic, tx_ptr, 2); for (int i=0; i<iic_wait; i++);
			
			// transfer rdac to eeprom
			tx_ptr[0] = 0xc0;
			tx_status = XIic_MasterSend(&Iic, tx_ptr, 1); for (int i=0; i<iic_wait; i++);
			regptr[DPOT_CONTROL] = 0x0100; // clear the semaphore
		}
		
		xil_printf("FPGA_VERSION = 0x%08x\n\r", regptr[FPGA_VERSION]);

        r1=4.99; r2=8.06;
        vmon_3v3 = 2.5 * (((regptr[AMC1_ADC7_6] >> 0) & 0x00000fff)/4096.0) * (r1+r2)/r1;
        r1=4.99; r2=15.0;
        vmon_5v0 = 2.5 * (((regptr[AMC1_ADC1_0] >> 16) & 0x00000fff)/4096.0) * (r1+r2)/r1;
        r1=4.99; r2=68.1;
        vmon_12v = 2.5 * (((regptr[AMC1_ADC3_2] >> 0) & 0x00000fff)/4096.0) * (r1+r2)/r1;
        r1=4.99; r2=68.1;
        vmon_24v = 2.5 * (((regptr[AMC1_ADC3_2] >> 16) & 0x00000fff)/4096.0) * (r1+r2)/r1;
        xil_printf("3v3 = %d mV, 5v0 = %d mV, 12v = %d mV, 24v = %d mV\n\r", (int)(vmon_3v3*1000.0), (int)(vmon_5v0*1000.0), (int)(vmon_12v*1000.0), (int)(vmon_24v*1000.0));

        //tempmon0 = 1.6*((regptr[AMC0_GPIO_ADC8] >> 0) & 0x0fff) - 273.0;
        //tempmon1 = 1.6*((regptr[AMC1_GPIO_ADC8] >> 0) & 0x0fff) - 273.0;
        //xil_printf("tempmon0 = %d, tempmon1 = %d\n\r", (int)tempmon0, (int)tempmon1);



		for (int i=0; i<10000000; i++); // wait
		
		whilecount++;
	}
	
	return(0);

}


static void RecvHandler(void *CallbackRef, int ByteCount)
{
	HandlerInfo.RemainingRecvBytes = ByteCount;
	HandlerInfo.RecvBytesUpdated = TRUE;
}

static void SendHandler(void *CallbackRef, int ByteCount)
{
	HandlerInfo.RemainingSendBytes = ByteCount;
	HandlerInfo.SendBytesUpdated = TRUE;
}

static void StatusHandler(void *CallbackRef, int Status)
{
	HandlerInfo.EventStatus |= Status;
	HandlerInfo.EventStatusUpdated = TRUE;
}

