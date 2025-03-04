
#include <stdio.h>
#include <stdlib.h>
#include "xil_printf.h"
#include "xparameters.h"
#include "fpga.h"


int main()
{

	uint32_t* gpioptr = (uint32_t *) XPAR_GPIO_0_BASEADDR;

    xil_printf("Hello World\n\r");

    uint32_t whilecount = 0;
    while(1) {

    	xil_printf("hello\n\r");
    	*gpioptr = whilecount;


    	for(int i=0; i<8000000; i++); // delay
    	whilecount++;

    }
    
    return 0;
}
