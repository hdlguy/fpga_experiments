#include <stdlib.h>
#include "xil_printf.h"
#include "xparameters.h"
#include "fpga.h"
#include "sleep.h"

int main()
{

    uint32_t* regptr = (uint32_t *) XPAR_XBRAM_2_BASEADDR;
    uint8_t* charptr = (uint8_t *) XPAR_XBRAM_2_BASEADDR;

    uint32_t whilecount=0;
    uint32_t temp=0;
    uint8_t  chartemp=0;
    while(1) {

    	for (int i=0; i<N_REGS; i++) regptr[i] = whilecount+i;
    	for (int i=0; i<N_REGS; i++) { temp ^= regptr[i]; }
    	
    	for (int i=0; i<N_REGS*4; i++) charptr[i] = whilecount+i;
    	for (int i=0; i<N_REGS*4; i++) { chartemp ^= charptr[i]; }

        xil_printf("0x%08x 0x%02x\n\r", temp, chartemp);

        usleep(1000);
    	whilecount++;

    }
    
    return 0;
}
