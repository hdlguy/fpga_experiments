//
#include <stdint.h>
#include <xil_printf.h>
#include "xparameters.h"
#include "fpga.h"

#define BRAM_SIZE (1 + XPAR_AXI_BRAM_CTRL_1_HIGHADDR - XPAR_AXI_BRAM_CTRL_1_BASEADDR) // bram size in bytes

int main()
{
    uint32_t* bramptr = (uint32_t *)XPAR_AXI_BRAM_CTRL_1_BASEADDR;
    
    xil_printf("bramptr = 0x%08x, BRAM_SIZE = 0x%04x\n\r", bramptr, BRAM_SIZE);

    uint32_t whilecount=0;
    while(1) {

        if (bramptr[0] != 0) {
            xil_printf("0x%08x: bramptr[0] = 0x%08x, clearing now.\n\r", whilecount, bramptr[0]);
            bramptr[0] = 0;
        }
        
        whilecount++;

    }

    return 0;

}


