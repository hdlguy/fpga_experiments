#include <stdlib.h>
#include "xil_printf.h"
#include "xparameters.h"
#include "fpga.h"

uint32_t wval[BRAM_SIZE/4], rval[BRAM_SIZE/4];
    
int main()
{

    uint32_t* regptr = (uint32_t *)XPAR_REGFILE_CTRL_BASEADDR;
   	uint32_t* bram_ptr = (uint32_t *)XPAR_AXI_BRAM_CTRL_0_BASEADDR;

    xil_printf("Hello World\n\r");

    uint32_t whilecount=0;
    while(1) {

    	xil_printf("\n\r0x%08x: FPGA_VERSION = 0x%08x, FPGA_ID = 0x%08x\n\r", whilecount, regptr[FPGA_VERSION], regptr[FPGA_ID]);

    	// *********** test the flash bram
    	// fill the bram
    	srand(whilecount);
    	for (int i=0; i<BRAM_SIZE/4; i++) wval[i] = (uint32_t)rand();
		for (int i=0; i<BRAM_SIZE/4; i++) bram_ptr[i] = wval[i];
    	// read and check the bram
    	uint32_t errors=0;
    	srand(whilecount);
    	for (int i=0; i<BRAM_SIZE/4; i++) rval[i] = bram_ptr[i];
		for (int i=0; i<BRAM_SIZE/4; i++) if (wval[i] != rval[i]) errors++;
    	xil_printf("bram_errors = %d\n\r", errors);
    	
    	
    	// write to the register file
    	for (int i=2; i<N_REGS; i++) regptr[i] = whilecount+i;
    	// read and check the register file
		errors = 0;
    	for (int i=2; i<N_REGS; i++) xil_printf("%d: %08x\n\r", i, regptr[i]);
    	for (int i=2; i<N_REGS; i++) { if (regptr[i] != (whilecount+i)) errors++; }
    	xil_printf("register errors = %d\n\r", errors);
    	

		// delay
    	for(int i=0; i<8000000; i++); 
    	whilecount++;

    }
    
    return 0;
}
