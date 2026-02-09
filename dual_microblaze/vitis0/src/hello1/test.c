
#include <stdio.h>
#include <stdlib.h>
#include "xil_printf.h"
#include "xparameters.h"
#include "fpga.h"


int main()
{

	uint32_t* gpioptr = (uint32_t *) XPAR_AXI_GPIO_0_BASEADDR;

    xil_printf("microblaze_0: Hello World!\n\r");

    uint32_t whilecount = 0;
    while(1) {

    	xil_printf("MB0: 0x%08x\n\r", whilecount);
    	*gpioptr = 0x01 << (whilecount & 0x00000003);


    	for(int i=0; i<2000000; i++); // delay
    	whilecount++;

    }
    
    return 0;
}

