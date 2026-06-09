#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include "fpga.h"
#include "xhwicap_l.h"
#include "xhwicap_i.h"

#define IPROG_LENGTH 8
static uint32_t IProg[IPROG_LENGTH] = {
		XHI_DUMMY_PACKET,
		XHI_SYNC_PACKET,
		XHI_NOOP_PACKET,
		0x30020001,
		0x00000000,
		0x30008001,
		XHI_CMD_IPROG,
		XHI_NOOP_PACKET
};

int main(int argc,char** argv)
{
    char devstr[] = "/dev/xdma0_bypass";

    // get pointers to the FPGA logic.
    void* base_addr;
    int fd = open(devstr, O_RDWR|O_SYNC);
    if(fd < 0) {
        fprintf(stderr,"Can't open %s, you must be root!\n", devstr);
    } else {
        base_addr = mmap(0,FPGA_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
        if(base_addr == NULL) fprintf(stderr,"Can't mmap\n");
    }
    //printf("FPGA_BASE_ADDRESS = 0x%08lx, virtual base_addr = %p\n", (uint64_t)FPGA_BASE_ADDRESS, base_addr);
    uint32_t *reg_ptr  = base_addr + FPGA_REG_OFFSET;
    printf("FPGA_ID = 0x%08x, FPGA_VERSION = 0x%08x\n", reg_ptr[FPGA_ID], reg_ptr[FPGA_VERSION]);


    uint32_t *icap_ptr  = base_addr + FPGA_ICAP_OFFSET;
	// write to the AXI HWICAP fifo
	for (int Index = 0; Index < IPROG_LENGTH; Index++) icap_ptr[XHI_WF_OFFSET/4] = IProg[Index];

	printf("************* hit ENTER to reboot from flash *************\n\r");
	getchar();

	// initiation tranfer
	printf("***********initiating IProg transfer. Hasta la vista! ***************\n\r");
    usleep(1000000);
	icap_ptr[XHI_CR_OFFSET/4] = XHI_CR_WRITE_MASK;




    reg_ptr[FPGA_LED] += 1;

    munmap(base_addr, FPGA_SIZE);

    return 0;
}


/*

#define IPROG_LENGTH 8
static u32 IProg[IPROG_LENGTH] = {
		XHI_DUMMY_PACKET,
		XHI_SYNC_PACKET,
		XHI_NOOP_PACKET,
		0x30020001,
		0x00000000,
		0x30008001,
		XHI_CMD_IPROG,
		XHI_NOOP_PACKET
};

int main(void)
{

	u32 BaseAddress = XPAR_HWICAP_0_BASEADDR;

	// write to the AXI HWICAP fifo
	for (int Index = 0; Index < IPROG_LENGTH; Index++) XHwIcap_WriteReg(BaseAddress, XHI_WF_OFFSET, IProg[Index]);

	xil_printf("************* hit ENTER to reboot from flash *************\n\r");
	getchar();

	// initiation tranfer
	xil_printf("***********initiating IProg transfer. Hasta la vista! ***************\n\r");
	for (int i=0; i<1000000; i++);
	XHwIcap_WriteReg(BaseAddress, XHI_CR_OFFSET, XHI_CR_WRITE_MASK);

	// we never get here
	uint32_t whilecount=0;
	while(1) {
		xil_printf("%d\n\r", whilecount);
		for (int i=0; i<10000000; i++); // wait		
		whilecount++;
	}
	
	return(0);

}
*/
