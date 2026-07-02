
#include "xparameters.h"
#include "xstatus.h"
#include "xuartlite_l.h"
#include "xil_printf.h"
#include "fpga.h"
#include <sleep.h>

#define TEST_BUFFER_SIZE 10


u8 SendBuffer[TEST_BUFFER_SIZE]; /* Buffer for Transmitting Data */
u8 RecvBuffer[TEST_BUFFER_SIZE]; /* Buffer for Receiving Data */

void uart_sendchar(uint32_t* uartptr, uint8_t data) {
	// wait for tx fifo not full
	while((uartptr[XUL_STATUS_REG_OFFSET/4] & 0x08) != 0);
	// write to tx fifo
	uartptr[XUL_TX_FIFO_OFFSET/4] = (uint32_t) data;
}

int uart_receivechar(uint32_t* uartptr, uint8_t* dptr) {
	int rxcount=0;
	while (((uartptr[XUL_STATUS_REG_OFFSET/4])&0x01) != 0) {		
		dptr[rxcount] = uartptr[XUL_RX_FIFO_OFFSET/4];
		rxcount++;
	}

	return(rxcount);
}

int main()
{
	int Index, numrx;
	uint32_t* uartptr = (uint32_t *)XPAR_AXI_UARTLITE_0_BASEADDR; 
	uint32_t* regptr  = (uint32_t *)XPAR_REGFILE_CTRL_BASEADDR; 

	// initialize buffers
	for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
		SendBuffer[Index] = Index + '0';
		RecvBuffer[Index] = 0;
	}


	// reset the uart fifos
	uartptr[XUL_CONTROL_REG_OFFSET/4] = 0x03;

	// send text
	uart_sendchar(uartptr, '\n'); uart_sendchar(uartptr, '\r');
	for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
		uart_sendchar(uartptr, SendBuffer[Index]);
	}

	while(1) {
		numrx = uart_receivechar(uartptr, RecvBuffer);
		regptr[FPGA_LED_CONTROL] += numrx;

		uart_sendchar(uartptr, 'H'); uart_sendchar(uartptr, 'Q'); uart_sendchar(uartptr, '\n'); uart_sendchar(uartptr, '\r');

		usleep(1000000);		
	}

	// // Check for a character and send it out
	// uart_sendchar(uartptr, '\n'); uart_sendchar(uartptr, '\r');
	// uint8_t rxchar;
	// while(1) {
		
	// 	if (((uartptr[XUL_STATUS_REG_OFFSET/4])&0x01) != 0) {
	// 		rxchar = uartptr[XUL_RX_FIFO_OFFSET];
	// 		uart_sendchar(uartptr, rxchar);
	// 		regptr[FPGA_LED_CONTROL] += 1;
	// 	}		
	// }

	return XST_SUCCESS;
}
