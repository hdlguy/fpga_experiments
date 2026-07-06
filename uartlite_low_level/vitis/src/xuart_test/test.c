#include "xparameters.h"
#include "xuartlite.h"
#include "xil_exception.h"
#include "xil_printf.h"
#include "xinterrupt_wrap.h"

#define XUARTLITE_BASEADDRESS	XPAR_XUARTLITE_0_BASEADDR

#define TEST_BUFFER_SIZE        4

int UartLiteIntrExample(XUartLite *UartLiteInstancePtr, UINTPTR BaseAddress);

void SendHandler(void *CallBackRef, unsigned int EventData);

void RecvHandler(void *CallBackRef, unsigned int EventData);

XUartLite UartLite; /* The instance of the UartLite Device */

//  * The following variables are shared between non-interrupt processing and interrupt processing such that they must be global.

//  * The following buffers are used in this example to send and receive data with the UartLite.
u8 SendBuffer[TEST_BUFFER_SIZE];
u8 ReceiveBuffer[TEST_BUFFER_SIZE];

// * The following counters are used to determine when the entire buffer has been sent and received.
static volatile int TotalReceivedCount;
static volatile int TotalSentCount;


// int UartLiteIntrExample(XUartLite *UartLiteInstPtr, UINTPTR BaseAddress)
int main(void)
{
	int Status;
	int Index;
	XUartLite *UartLiteInstPtr = &UartLite;
	UINTPTR BaseAddress = XUARTLITE_BASEADDRESS;

	XUartLite_Config *CfgPtr;
	(void)UartLiteInstPtr;

	//  * Initialize the UartLite driver so that it's ready to use.
	CfgPtr = XUartLite_LookupConfig(BaseAddress);
	Status = XUartLite_Initialize(&UartLite, BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	//  * Perform a self-test to ensure that the hardware was built correctly.
	Status = XUartLite_SelfTest(&UartLite);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	//  * Connect the UartLite to the interrupt subsystem such that interrupts can occur. This function is application specific.
	Status = XSetupInterruptSystem(&UartLite, &XUartLite_InterruptHandler, CfgPtr->IntrId, CfgPtr->IntrParent, XINTERRUPT_DEFAULT_PRIORITY);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Setup the handlers for the UartLite that will be called from the
	 * interrupt context when data has been sent and received, specify a
	 * pointer to the UartLite driver instance as the callback reference so
	 * that the handlers are able to access the instance data.
	 */
	XUartLite_SetSendHandler(&UartLite, SendHandler, &UartLite);
	XUartLite_SetRecvHandler(&UartLite, RecvHandler, &UartLite);

	//* Enable the interrupt of the UartLite so that interrupts will occur.
	XUartLite_EnableInterrupt(&UartLite);

	/*
	 * Initialize the send buffer bytes with a pattern to send and the
	 * the receive buffer bytes to zero to allow the receive data to be
	 * verified.
	 */
	for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
		SendBuffer[Index] = Index+0x30;
		ReceiveBuffer[Index] = 0;
	}

	//* Start receiving data before sending it since there is a loopback.
	XUartLite_Recv(&UartLite, ReceiveBuffer, TEST_BUFFER_SIZE);

	//  * Send the buffer using the UartLite.
	XUartLite_Send(&UartLite, SendBuffer, TEST_BUFFER_SIZE);

	/*
	 * Wait for the entire buffer to be received, letting the interrupt
	 * processing work in the background, this function may get locked
	 * up in this loop if the interrupts are not working correctly.
	 */
	while (TotalReceivedCount != TEST_BUFFER_SIZE) { 	}

	xil_printf("\n\rchars detected\n\r");

	// * Verify the entire receive buffer was successfully received.
	for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
		if (ReceiveBuffer[Index] != SendBuffer[Index]) {
			return XST_FAILURE;
		}
	}

	xil_printf("test passed\n\r");
	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function is the handler which performs processing to send data to the
* UartLite. It is called from an interrupt context such that the amount of
* processing performed should be minimized. It is called when the transmit
* FIFO of the UartLite is empty and more data can be sent through the UartLite.
*
* This handler provides an example of how to handle data for the UartLite,
* but is application specific.
*
* @param	CallBackRef contains a callback reference from the driver.
*		In this case it is the instance pointer for the UartLite driver.
* @param	EventData contains the number of bytes sent or received for sent
*		and receive events.
*
* @return	None.
*
* @note		None.
*
****************************************************************************/
void SendHandler(void *CallBackRef, unsigned int EventData)
{
	(void)CallBackRef;
	TotalSentCount = EventData;
}



/****************************************************************************/
/**
*
* This function is the handler which performs processing to receive data from
* the UartLite. It is called from an interrupt context such that the amount of
* processing performed should be minimized.  It is called data is present in
* the receive FIFO of the UartLite such that the data can be retrieved from
* the UartLite. The size of the data present in the FIFO is not known when
* this function is called.
*
* This handler provides an example of how to handle data for the UartLite,
* but is application specific.
*
* @param	CallBackRef contains a callback reference from the driver, in
*		this case it is the instance pointer for the UartLite driver.
* @param	EventData contains the number of bytes sent or received for sent
*		and receive events.
*
* @return	None.
*
* @note		None.
*
****************************************************************************/
void RecvHandler(void *CallBackRef, unsigned int EventData)
{
	(void)CallBackRef;
	TotalReceivedCount = EventData;
}


