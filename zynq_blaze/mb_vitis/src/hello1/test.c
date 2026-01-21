#include "xil_printf.h"
#include "xparameters.h"
#include "fpga.h"

#define BRAM_SIZE ((int)(XPAR_AXI_BRAM_CTRL_0_HIGHADDR-XPAR_AXI_BRAM_CTRL_0_BASEADDR+1))
    
int main()
{

   	uint32_t* bram_ptr = (uint32_t *)XPAR_AXI_BRAM_CTRL_0_BASEADDR;

    xil_printf("Hello World\n\r");

    uint32_t whilecount=0;
    while(1) {

    	// *********** test the flash bram
    	// fill the bram
    	for (int i=0; i<BRAM_SIZE/4; i++) bram_ptr[i] = (whilecount+i); //(uint32_t)rand();
    	// read and check the bram
    	uint32_t errors=0;
		for (int i=0; i<BRAM_SIZE/4; i++) if (bram_ptr[i] != (whilecount+i)) errors++;
    	xil_printf("bram_errors = %d\n\r", errors);
    	
		// delay
    	for(int i=0; i<8000000; i++); 
    	whilecount++;

    }
    
    return 0;
}
