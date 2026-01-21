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

		xil_printf("0x%08x\n", bram_ptr[0]);
		bram_ptr[0] = 0xffffffff;
	
		// delay
    	for(int i=0; i<8000000; i++); 
    	whilecount++;

    }
    
    return 0;
}
