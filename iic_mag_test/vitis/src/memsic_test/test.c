//
#include "xparameters.h"
#include "xiic.h"
#include "sleep.h"

#ifndef SDT

#ifdef XPAR_INTC_0_DEVICE_ID
#include "xintc.h"
#else
#include "xscugic.h"
#endif

#endif

#include "xil_exception.h"
#include "xil_printf.h"

#ifdef SDT
#include "xinterrupt_wrap.h"
#endif

#ifndef SDT
#define IIC_DEVICE_ID		XPAR_IIC_0_DEVICE_ID
#else
#define	XIIC_BASEADDRESS	XPAR_XIIC_0_BASEADDR
#endif

#ifndef SDT

#ifdef XPAR_INTC_0_DEVICE_ID
#define INTC_DEVICE_ID	XPAR_INTC_0_DEVICE_ID
#define IIC_INTR_ID	XPAR_INTC_0_IIC_0_VEC_ID
#define INTC			XIntc
#define INTC_HANDLER	XIntc_InterruptHandler
#else
#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID
#define IIC_INTR_ID		XPAR_FABRIC_IIC_0_VEC_ID
#define INTC			 	XScuGic
#define INTC_HANDLER		XScuGic_InterruptHandler
#endif

#endif

#define SLAVE_ADDRESS	0x30 // 7-bit address

#define SEND_COUNT		16
#define RECEIVE_COUNT   16


int IicRepeatedStartExample();


#ifndef SDT
static int SetupInterruptSystem(XIic *IicInstPtr);
#endif

static void SendHandler    (XIic *InstancePtr, int Event);
static void ReceiveHandler (XIic *InstancePtr, int Event);
static void StatusHandler  (XIic *InstancePtr, int Event);

XIic IicInstance;
uint8_t GlobalJunk;

#ifndef SDT
INTC Intc; 	/* The instance of the Interrupt Controller Driver */
#endif

u8 WriteBuffer[SEND_COUNT];	/* Write buffer for writing a page. */
u8 ReadBuffer[RECEIVE_COUNT];	/* Read buffer for reading a page. */

volatile u8 TransmitComplete;
volatile u8 ReceiveComplete;

#define MemsicXLow 		0x00
#define MemsicXHigh 	0x01
#define MemsicYLow 		0x02
#define MemsicYHigh 	0x03
#define MemsicZLow 		0x04
#define MemsicZHigh 	0x05
#define MemsicStatus 	0x06
#define MemsicControl0 	0x07
#define MemsicControl1 	0x08
#define MemsicR0 		0x1B
#define MemsicR1 		0x1C
#define MemsicR2 		0x1D
#define MemsicR3 		0x1E
#define MemsicR4 		0x1F
#define MemsicID 		0x20

void memsic_read(uint8_t startreg, int numregs, uint8_t* buf)
{
	// int Status;
	// int BusBusy;
	uint8_t wbuf[4], rbuf[16];
	
	XIic_SetAddress(&IicInstance, XII_ADDR_TO_SEND_TYPE, SLAVE_ADDRESS);

	TransmitComplete = 1;

	XIic_Start(&IicInstance);

	IicInstance.Options = XII_REPEATED_START_OPTION;

	wbuf[0] = startreg;
	XIic_MasterSend(&IicInstance, wbuf, 1); // send start register


	while (TransmitComplete) { }

	XIic_IsIicBusy(&IicInstance);

	ReceiveComplete = 1;
	IicInstance.Options = 0x0;

	XIic_MasterRecv(&IicInstance, rbuf, numregs);

	while ((ReceiveComplete) || (XIic_IsIicBusy(&IicInstance) == TRUE)) { }

	XIic_Stop(&IicInstance);

	for (int i=0; i<numregs; i++) { buf[i] = rbuf[i]; }
	
}

void memsic_write(uint8_t startreg, int numregs, uint8_t* buf)
{

	uint8_t wbuf[4];
	
	XIic_SetAddress(&IicInstance, XII_ADDR_TO_SEND_TYPE, SLAVE_ADDRESS);

	TransmitComplete = 1;

	XIic_Start(&IicInstance);

	IicInstance.Options = 0x0;

	wbuf[0] = startreg;
	for(int i=0; i<numregs; i++) { wbuf[i+1] = buf[i]; }
	XIic_MasterSend(&IicInstance, wbuf, numregs+1); // send start register

	while (TransmitComplete) { }
	
	XIic_IsIicBusy(&IicInstance); //??

	XIic_Stop(&IicInstance);

}


int main(void)
{
	// u8 Index;
	int Status;
	XIic_Config *ConfigPtr;	/* Pointer to configuration data */

	xil_printf("\n\r**** memsic_test ****\n\r");


#ifndef SDT
	ConfigPtr = XIic_LookupConfig(XPAR_IIC_0_DEVICE_ID);
#else
	ConfigPtr = XIic_LookupConfig(XIIC_BASEADDRESS);
#endif
	if (ConfigPtr == NULL) { return XST_FAILURE;  }

	Status = XIic_CfgInitialize(&IicInstance, ConfigPtr, ConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS) { return XST_FAILURE; }


#ifndef SDT
	Status = SetupInterruptSystem(&IicInstance);
#else
	Status = XSetupInterruptSystem(&IicInstance, &XIic_InterruptHandler, ConfigPtr->IntrId, ConfigPtr->IntrParent, XINTERRUPT_DEFAULT_PRIORITY); 
#endif
	if (Status != XST_SUCCESS) { return XST_FAILURE; }


	XIic_SetSendHandler  (&IicInstance, &IicInstance, (XIic_Handler) SendHandler);
	XIic_SetRecvHandler  (&IicInstance, &IicInstance, (XIic_Handler) ReceiveHandler);
	XIic_SetStatusHandler(&IicInstance, &IicInstance, (XIic_StatusHandler) StatusHandler);
	
	// configure the memsic
	WriteBuffer[0] = 0x0e;
	WriteBuffer[1] = 0x00;
	memsic_write(MemsicControl0, 2, WriteBuffer);	

	uint32_t whilecount=0;
	uint8_t memsic_id, memsic_status;
	int16_t mag_x=0, mag_y=0, mag_z=0;
	while(1) {

		xil_printf("\n\rwhilecount = 0x%08x\n\r", whilecount);
		
		// print ID
		memsic_read(MemsicID, 1, ReadBuffer);
		memsic_id = ReadBuffer[0]; 
		xil_printf("memsic_id = 0x%02x\n\r", memsic_id);

		// print Status
		memsic_read(MemsicStatus, 1, ReadBuffer);
		memsic_status = ReadBuffer[0]; 
		xil_printf("memsic_status = 0x%02x\n\r", memsic_status);

		// print XYZ magnetic field
		memsic_read(MemsicXLow, 6, ReadBuffer);
		for(int i=0; i<6; i++) { xil_printf("0x%02x ", ReadBuffer[i]); } xil_printf("\n\r");
		mag_x = (((uint16_t)ReadBuffer[1]) << 8) | (ReadBuffer[0]);		
		mag_y = (((uint16_t)ReadBuffer[3]) << 8) | (ReadBuffer[2]);
		mag_z = (((uint16_t)ReadBuffer[5]) << 8) | (ReadBuffer[4]);
		xil_printf("mag_x = %05d, mag_y = %05d, mag_z = %05d\n\r", mag_x, mag_y, mag_z);
		
		// // command the next conversion
		// WriteBuffer[0] = 0x01;
		// memsic_write(MemsicControl0, 1, WriteBuffer);		

		whilecount++;
		usleep(1000000);

	}

	return XST_SUCCESS;
}


#ifndef SDT


static int SetupInterruptSystem(XIic *IicInstPtr)
{
	int Status;

#ifdef XPAR_INTC_0_DEVICE_ID


	Status = XIntc_Initialize(&Intc, INTC_DEVICE_ID);

	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	Status = XIntc_Connect(&Intc, IIC_INTR_ID, (XInterruptHandler) XIic_InterruptHandler, IicInstPtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	Status = XIntc_Start(&Intc, XIN_REAL_MODE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	XIntc_Enable(&Intc, IIC_INTR_ID);

#else

	XScuGic_Config *IntcConfig;

	IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
	if (NULL == IntcConfig) {
		return XST_FAILURE;
	}

	Status = XScuGic_CfgInitialize(&Intc, IntcConfig, IntcConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	XScuGic_SetPriorityTriggerType(&Intc, IIC_INTR_ID, 0xA0, 0x3);

	Status = XScuGic_Connect(&Intc, IIC_INTR_ID, (Xil_InterruptHandler)XIic_InterruptHandler, IicInstPtr);
	if (Status != XST_SUCCESS) {
		return Status;
	}


	XScuGic_Enable(&Intc, IIC_INTR_ID);

#endif


	Xil_ExceptionInit();

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler) INTC_HANDLER, &Intc);

	Xil_ExceptionEnable();

	return XST_SUCCESS;
}
#endif


static void SendHandler(XIic *InstancePtr, int Event)
{
	if ((InstancePtr==NULL) && (Event==0)) GlobalJunk=0; // to shut up compiler
	
	TransmitComplete = 0;
}


static void ReceiveHandler(XIic *InstancePtr, int Event)
{
	if ((InstancePtr==NULL) && (Event==0)) GlobalJunk=0; // to shut up compiler
	
	ReceiveComplete = 0;
}


static void StatusHandler(XIic *InstancePtr, int Event)
{
	
	if ((InstancePtr==NULL) && (Event==0)) GlobalJunk=0; // to shut up compiler

}
