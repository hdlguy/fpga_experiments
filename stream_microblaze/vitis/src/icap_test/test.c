#include <xparameters.h>
#include <xstatus.h>
#include <xil_types.h>
#include <xil_assert.h>
#include <xhwicap_i.h>
#include <xhwicap_l.h>
#include <stdio.h>
#include "xil_printf.h"

//#include "fpga.h"

#define IPROG_LENGTH 8
static u32 IProg[IPROG_LENGTH] = {
		XHI_DUMMY_PACKET,
		XHI_SYNC_PACKET,
		XHI_NOOP_PACKET,
		0x30020001,
		0x00000000,
		0x30008001,
		XHI_CMD_IPROG,
		XHI_NOOP_PACKET
};

int main(void)
{

	u32 BaseAddress = XPAR_HWICAP_0_BASEADDR;

	// write to the AXI HWICAP fifo
	for (int Index = 0; Index < IPROG_LENGTH; Index++) XHwIcap_WriteReg(BaseAddress, XHI_WF_OFFSET, IProg[Index]);

	xil_printf("************* hit ENTER to reboot from flash *************\n\r");
	getchar();

	// initiation tranfer
	xil_printf("***********initiating IProg transfer. Hasta la vista! ***************\n\r");
	for (int i=0; i<1000000; i++);
	XHwIcap_WriteReg(BaseAddress, XHI_CR_OFFSET, XHI_CR_WRITE_MASK);

	// we never get here
	uint32_t whilecount=0;
	while(1) {
		xil_printf("%d\n\r", whilecount);
		for (int i=0; i<10000000; i++); // wait		
		whilecount++;
	}
	
	return(0);

}


