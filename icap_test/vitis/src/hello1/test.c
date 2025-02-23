
#include <stdio.h>
#include <stdlib.h>
#include "xil_printf.h"
#include "xparameters.h"
#include "fpga.h"


int main()
{

    uint32_t *regptr = (uint32_t *)REG_BASEADDR;
	uint32_t* flash_bram_ptr = (uint32_t *)XPAR_BRAM_0_BASEADDR;

    xil_printf("Hello World\n\r");

	uint32_t wval[FLASH_BRAM_SIZE/4], rval[FLASH_BRAM_SIZE/4];
    uint32_t whilecount=0;
    while(1) {

    	xil_printf("0x%08x: FPGA_VERSION = 0x%08x, FPGA_ID = 0x%08x\n\r", whilecount, regptr[FPGA_VERSION], regptr[FPGA_ID]);

    	// test the flash bram
    	// fill the bram
    	srand(whilecount);
    	for (int i=0; i<FLASH_BRAM_SIZE/4; i++) wval[i] = (uint32_t)rand();
		for (int i=0; i<FLASH_BRAM_SIZE/4; i++) flash_bram_ptr[i] = wval[i];

    	// read and check the bram
    	uint32_t errors=0;
    	srand(whilecount);
    	for (int i=0; i<FLASH_BRAM_SIZE/4; i++) rval[i] = flash_bram_ptr[i];
		for (int i=0; i<FLASH_BRAM_SIZE/4; i++) if (wval[i] != rval[i]) errors++;
		for (int i=0; i<8; i++) xil_printf("%08x: %08x  %08x\n\r", i, wval[i], rval[i]);
    	xil_printf("flash_bram_errors = %d\n\r", errors);


    	for(int i=0; i<8000000; i++); // delay
    	whilecount++;

    }
    
    return 0;
}
