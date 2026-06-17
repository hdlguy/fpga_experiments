#include <stdlib.h>
#include "xil_printf.h"
#include "xparameters.h"
#include "fpga.h"

int main()
{

    uint32_t* regptr = (uint32_t *) XPAR_XBRAM_2_BASEADDR; //XPAR_REGFILE_CTRL_S_AXI_BASEADDR;

    uint32_t whilecount=0;
    uint32_t temp=0;
    while(1) {

    	for (int i=0; i<N_REGS; i++) regptr[i] = whilecount+i;
    	for (int i=0; i<N_REGS; i++) { temp ^= regptr[i]; }
        xil_printf("0x%08x\n", temp);
    	
    	for(int i=0; i<8000; i++); // delay
    	whilecount++;

    }
    
    return 0;
}
