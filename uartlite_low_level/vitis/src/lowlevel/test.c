
#include "xparameters.h"
#include "xstatus.h"
#include "xuartlite_l.h"
#include "xil_printf.h"
#include "fpga.h"

#ifndef SDT
#define UARTLITE_BASEADDR	   XPAR_UARTLITE_0_BASEADDR
#else
#define UARTLITE_BASEADDR	   XPAR_XUARTLITE_0_BASEADDR
#endif

#define TEST_BUFFER_SIZE 16


#ifndef SDT
int UartLiteLowLevelExample(u32 UartliteBaseAddress);
#else
int UartLiteLowLevelExample(UINTPTR BaseAddress);
#endif
u8 SendBuffer[TEST_BUFFER_SIZE]; /* Buffer for Transmitting Data */
u8 RecvBuffer[TEST_BUFFER_SIZE]; /* Buffer for Receiving Data */


int main(void)
{
	int Status;

	/*
	 * Run the UartLite Low level example, specify the BaseAddress that is
	 * generated in xparameters.h.
	 */
	Status = UartLiteLowLevelExample(UARTLITE_BASEADDR);
	if (Status != XST_SUCCESS) {
		xil_printf("Uartlite lowlevel Example Failed\r\n");
		return XST_FAILURE;
	}

	xil_printf("Successfully ran Uartlite lowlevel Example\r\n");
	return XST_SUCCESS;
}


#ifndef SDT
int UartLiteLowLevelExample(u32 UartliteBaseAddress)
#else
int UartLiteLowLevelExample(UINTPTR UartliteBaseAddress)
#endif
{
	int Index;
	uint32_t* uartptr = (uint32_t *)XPAR_AXI_UARTLITE_0_BASEADDR; 
	uint32_t* regptr = (uint32_t *)XPAR_AXI_UARTLITE_0_BASEADDR; 

	/*
	 * Initialize the send buffer bytes with a pattern to send and the
	 * the receive buffer bytes to zero.
	 */
	for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
		SendBuffer[Index] = Index + 'B';
		RecvBuffer[Index] = 0;
	}


	for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
		XUartLite_SendByte(UartliteBaseAddress, SendBuffer[Index]);
	}


	XUartLite_SendByte(UartliteBaseAddress, '\n');
	XUartLite_SendByte(UartliteBaseAddress, '\r');
	uint8_t rxchar;
	while(1) {
		if (((uartptr[XUL_STATUS_REG_OFFSET])&0x01) != 0) {
			rxchar = uartptr[XUL_RX_FIFO_OFFSET];
			uartptr[XUL_TX_FIFO_OFFSET] = rxchar;
		}
		
		regptr[FPGA_LED_CONTROL] += 1;
	}

	return XST_SUCCESS;
}
