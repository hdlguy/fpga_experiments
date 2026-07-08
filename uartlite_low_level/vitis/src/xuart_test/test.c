#include "xparameters.h"
#include "xuartlite.h"
#include "xuartlite_l.h"
#include "xil_exception.h"
#include "xil_printf.h"
#include "xinterrupt_wrap.h"
#include "sleep.h"
#include <stdbool.h>
#include "fpga.h"

#define XUARTLITE_BASEADDRESS	XPAR_XUARTLITE_0_BASEADDR

#define TEST_BUFFER_SIZE       20


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

	// //  * Perform a self-test to ensure that the hardware was built correctly.
	// Status = XUartLite_SelfTest(&UartLite);
	// if (Status != XST_SUCCESS) {
	// 	return XST_FAILURE;
	// }

	//  * Connect the UartLite to the interrupt subsystem such that interrupts can occur. This function is application specific.
	Status = XSetupInterruptSystem(&UartLite, &XUartLite_InterruptHandler, CfgPtr->IntrId, CfgPtr->IntrParent, XINTERRUPT_DEFAULT_PRIORITY);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/* Setup the handlers for the UartLite that will be called from the
	 * interrupt context when data has been sent and received, specify a
	 * pointer to the UartLite driver instance as the callback reference so
	 * that the handlers are able to access the instance data. */
	XUartLite_SetSendHandler(&UartLite, SendHandler, &UartLite);
	XUartLite_SetRecvHandler(&UartLite, RecvHandler, &UartLite);

	//* Enable the interrupt of the UartLite so that interrupts will occur.
	XUartLite_EnableInterrupt(&UartLite);

	/* Initialize the send buffer bytes with a pattern to send and the
	 * the receive buffer bytes to zero to allow the receive data to be
	 * verified.	 */
	for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
		SendBuffer[Index] = Index+0x30;
		ReceiveBuffer[Index] = 0;
	}

	//* Start receiving data before sending it since there is a loopback.
	XUartLite_Recv(&UartLite, ReceiveBuffer, 1);


	uint32_t whilecount = 0;
	char rxchar;
	bool ui_active = FALSE;
	while(1) {

		// print the periodic status message
		if (!ui_active) {
			xil_printf("0x%08x\n\r", whilecount);
		}

		// a character is recieved from the console
		if (TotalReceivedCount > 0) {

			rxchar = ReceiveBuffer[0];
			switch(rxchar) {

				case 'h':
				case 'H':
				case '?':
					ui_active = TRUE;
					xil_printf(
						"\n\rHelp:\n\r\th or H or ? = this help message\n\r" 
						"\tm or M = set MAC address, format: \"m 00:d8:61:59:63:7a\"\n\r"
						"\ti or I = set IP address, format: \"i 16.0.0.128\"\n\r"
						"\te or E = exit menu\n\r"
					);
					break;

				case 'm':
				case 'M':
					xil_printf("MAC address = 00:d8:61:59:63:7a\n\r");
					break;

				case 'i':
				case 'I':
					xil_printf("IP address = 16.0.0.128\n\r");
					break;

				case 'e':
				case 'E':
					ui_active = FALSE;
					xil_printf("Exit Menu\n\r\n\r");
					break;

			}
			
			TotalReceivedCount = 0;
			XUartLite_Recv(&UartLite, ReceiveBuffer, 1);
		}

		// this code corresponds to the normal operations that we don't want blocked.
		whilecount++;
		usleep(1000000);
	}

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
* processing performed should be minimized.  It is called when data is present in
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
	uint32_t *regptr  = (uint32_t *)XPAR_REGFILE_CTRL_BASEADDR;
	//uint32_t *uartptr = (uint32_t *)XPAR_AXI_UARTLITE_0_BASEADDR;

	regptr[FPGA_LED_CONTROL]++;
	// regptr[FPGA_LED_CONTROL] = uartptr[XUL_RX_FIFO_OFFSET/4];
	// regptr[FPGA_LED_CONTROL] = TotalReceivedCount & 0x00ff;
	
}


