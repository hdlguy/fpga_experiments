//
#include "xparameters.h"
#include "xiic.h"

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

#define SEND_COUNT		2
#define RECEIVE_COUNT   2


int IicRepeatedStartExample();
static int WriteData(u16 ByteCount);
static int ReadData(u8 *BufferPtr, u16 ByteCount);

#ifndef SDT
static int SetupInterruptSystem(XIic *IicInstPtr);
#endif

static void SendHandler(XIic *InstancePtr);
static void ReceiveHandler(XIic *InstancePtr);
static void StatusHandler(XIic *InstancePtr, int Event);

XIic IicInstance;

#ifndef SDT
INTC Intc; 	/* The instance of the Interrupt Controller Driver */
#endif

u8 WriteBuffer[SEND_COUNT];	/* Write buffer for writing a page. */
u8 ReadBuffer[RECEIVE_COUNT];	/* Read buffer for reading a page. */

volatile u8 TransmitComplete;
volatile u8 ReceiveComplete;

uint8_t memsic_read_id()
{
	int Status;
	int BusBusy;
	uint8_t wbuf[4], rbuf[4];
	
	Status = XIic_SetAddress(&IicInstance, XII_ADDR_TO_SEND_TYPE, SLAVE_ADDRESS);

	TransmitComplete = 1;

	Status = XIic_Start(&IicInstance);

	IicInstance.Options = XII_REPEATED_START_OPTION;

	wbuf[0] = 0x20; // ID register
	Status = XIic_MasterSend(&IicInstance, wbuf, 1); // send register number


	while (TransmitComplete) { }

	BusBusy = XIic_IsIicBusy(&IicInstance);

	ReceiveComplete = 1;
	IicInstance.Options = 0x0;

	Status = XIic_MasterRecv(&IicInstance, rbuf, 1);

	while ((ReceiveComplete) || (XIic_IsIicBusy(&IicInstance) == TRUE)) { }

	Status = XIic_Stop(&IicInstance);

	return(rbuf[0]);	
	
}


// int IicRepeatedStartExample(void)
int main(void)
{
	u8 Index;
	int Status;
	XIic_Config *ConfigPtr;	/* Pointer to configuration data */


	for (Index = 0; Index < SEND_COUNT; Index++) {
		WriteBuffer[Index] = Index;
		ReadBuffer[Index] = 0;
	}


#ifndef SDT
	ConfigPtr = XIic_LookupConfig(XPAR_IIC_0_DEVICE_ID);
#else
	ConfigPtr = XIic_LookupConfig(XIIC_BASEADDRESS);
#endif
	if (ConfigPtr == NULL) {
		return XST_FAILURE;
	}

	Status = XIic_CfgInitialize(&IicInstance, ConfigPtr, ConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


#ifndef SDT
	Status = SetupInterruptSystem(&IicInstance);
#else
	Status = XSetupInterruptSystem(&IicInstance, &XIic_InterruptHandler,
				       ConfigPtr->IntrId, ConfigPtr->IntrParent,
				       XINTERRUPT_DEFAULT_PRIORITY);
#endif
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	XIic_SetSendHandler(&IicInstance, &IicInstance, (XIic_Handler) SendHandler);
	XIic_SetRecvHandler(&IicInstance, &IicInstance, (XIic_Handler) ReceiveHandler);
	XIic_SetStatusHandler(&IicInstance, &IicInstance, (XIic_StatusHandler) StatusHandler);

	uint8_t memsic_id;
	memsic_id = memsic_read_id();
	xil_printf("memsic_id = 0x%02x\n\r", memsic_id);

	return XST_SUCCESS;
}


static int WriteData(u16 ByteCount)
{
	int Status;
	int BusBusy;


	TransmitComplete = 1;


	Status = XIic_Start(&IicInstance);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	IicInstance.Options = XII_REPEATED_START_OPTION;


	Status = XIic_MasterSend(&IicInstance, WriteBuffer, ByteCount);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	while (TransmitComplete) {

	}


	BusBusy = XIic_IsIicBusy(&IicInstance);

	TransmitComplete = 1;
	IicInstance.Options = 0x0;


	Status = XIic_MasterSend(&IicInstance, WriteBuffer, ByteCount);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Wait till data is transmitted.
	 */
	while ((TransmitComplete) || (XIic_IsIicBusy(&IicInstance) == TRUE)) {

	}

	/*
	 * Stop the IIC device.
	 */
	Status = XIic_Stop(&IicInstance);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}


static int ReadData(u8 *BufferPtr, u16 ByteCount)
{
	int Status;
	int BusBusy;

	/*
	 * Set the defaults.
	 */
	ReceiveComplete = 1;

	/*
	 * Start the IIC device.
	 */
	Status = XIic_Start(&IicInstance);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Set the Repeated Start option.
	 */
	IicInstance.Options = XII_REPEATED_START_OPTION;

	/*
	 * Receive the data.
	 */
	Status = XIic_MasterRecv(&IicInstance, BufferPtr, ByteCount);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Wait till all the data is received.
	 */
	while (ReceiveComplete) {

	}

	/*
	 * This is for verification that Bus is not released and still Busy.
	 */
	BusBusy = XIic_IsIicBusy(&IicInstance);

	ReceiveComplete = 1;
	IicInstance.Options = 0x0;

	/*
	 * Receive the Data.
	 */
	Status = XIic_MasterRecv(&IicInstance, BufferPtr, ByteCount);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Wait till all the data is received.
	 */
	while ((ReceiveComplete) || (XIic_IsIicBusy(&IicInstance) == TRUE)) {

	}

	/*
	 * Stop the IIC device.
	 */
	Status = XIic_Stop(&IicInstance);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

#ifndef SDT
/*****************************************************************************/
/**
* This function setups the interrupt system so interrupts can occur for the
* IIC. The function is application-specific since the actual system may or
* may not have an interrupt controller. The IIC device could be directly
* connected to a processor without an interrupt controller. The user should
* modify this function to fit the application.
*
* @param	IicInstPtr contains a pointer to the instance of the IIC  which
*		is going to be connected to the interrupt controller.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
static int SetupInterruptSystem(XIic *IicInstPtr)
{
	int Status;

#ifdef XPAR_INTC_0_DEVICE_ID

	/*
	 * Initialize the interrupt controller driver so that it's ready to use.
	 */
	Status = XIntc_Initialize(&Intc, INTC_DEVICE_ID);

	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Connect the device driver handler that will be called when an
	 * interrupt for the device occurs, the handler defined above performs
	 * the specific interrupt processing for the device.
	 */
	Status = XIntc_Connect(&Intc, IIC_INTR_ID,
			       (XInterruptHandler) XIic_InterruptHandler,
			       IicInstPtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Start the interrupt controller so interrupts are enabled for all
	 * devices that cause interrupts.
	 */
	Status = XIntc_Start(&Intc, XIN_REAL_MODE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Enable the interrupts for the IIC device.
	 */
	XIntc_Enable(&Intc, IIC_INTR_ID);

#else

	XScuGic_Config *IntcConfig;

	/*
	 * Initialize the interrupt controller driver so that it is ready to
	 * use.
	 */
	IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
	if (NULL == IntcConfig) {
		return XST_FAILURE;
	}

	Status = XScuGic_CfgInitialize(&Intc, IntcConfig,
				       IntcConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	XScuGic_SetPriorityTriggerType(&Intc, IIC_INTR_ID,
				       0xA0, 0x3);

	/*
	 * Connect the interrupt handler that will be called when an
	 * interrupt occurs for the device.
	 */
	Status = XScuGic_Connect(&Intc, IIC_INTR_ID,
				 (Xil_InterruptHandler)XIic_InterruptHandler,
				 IicInstPtr);
	if (Status != XST_SUCCESS) {
		return Status;
	}

	/*
	 * Enable the interrupt for the IIC device.
	 */
	XScuGic_Enable(&Intc, IIC_INTR_ID);

#endif

	/*
	 * Initialize the exception table.
	 */
	Xil_ExceptionInit();

	/*
	 * Register the interrupt controller handler with the exception table.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
				     (Xil_ExceptionHandler) INTC_HANDLER,
				     &Intc);

	/*
	 * Enable non-critical exceptions.
	 */
	Xil_ExceptionEnable();

	return XST_SUCCESS;
}
#endif

/*****************************************************************************/
/**
* This Send handler is called asynchronously from an interrupt context and
* indicates that data in the specified buffer has been sent.
*
* @param	InstancePtr is a pointer to the IIC driver instance for which
* 		the handler is being called for.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
static void SendHandler(XIic *InstancePtr)
{
	TransmitComplete = 0;
}

/*****************************************************************************/
/**
* This Receive handler is called asynchronously from an interrupt context and
* indicates that data in the specified buffer has been Received.
*
* @param	InstancePtr is a pointer to the IIC driver instance for which
* 		the handler is being called for.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
static void ReceiveHandler(XIic *InstancePtr)
{
	ReceiveComplete = 0;
}

/*****************************************************************************/
/**
* This Status handler is called asynchronously from an interrupt
* context and indicates the events that have occurred.
*
* @param	InstancePtr is a pointer to the IIC driver instance for which
*		the handler is being called for.
* @param	Event indicates the condition that has occurred.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
static void StatusHandler(XIic *InstancePtr, int Event)
{

}
