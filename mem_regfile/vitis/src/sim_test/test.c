#include <stdlib.h>
#include "xil_printf.h"
#include "xparameters.h"
#include "fpga.h"

#define NREGS 16

int main()
{

    uint32_t* regptr = (uint32_t *)XPAR_REGFILE_CTRL_S_AXI_BASEADDR;

    uint32_t whilecount=0;
    uint32_t temp=0;
    while(1) {

    	for (int i=0; i<NREGS; i++) regptr[i] = whilecount+i;
    	for (int i=0; i<NREGS; i++) temp ^= regptr[i];
        xil_printf("0x%08x\n", temp);
    	
    	for(int i=0; i<8000; i++); // delay
    	whilecount++;

    }
    
    return 0;
}
