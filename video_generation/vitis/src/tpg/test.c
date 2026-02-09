#include "xil_printf.h"
#include "xparameters.h"
//#include "fpga.h"
#include "xv_tpg_hw.h"

int main()
{

    xil_printf("Video Test Pattern Genertor, TPG\n\r");
    
    // uint32_t *regptr = (uint32_t *)XPAR_M00_AXI_BASEADDR;
    //uint32_t *regptr = (uint32_t *)XPAR_AXI_REGFILE_CTRL_BASEADDR;
    //xil_printf("regptr = %p\n\r", regptr);

    uint32_t *tpgptr = (uint32_t *)XPAR_V_TPG_0_BASEADDR;
    xil_printf("tpgptr = %p\n\r", tpgptr);

    uint32_t rval;
    // for (int i=0; i<55; i++) {
    //     rval = tpgptr[i];
    //     xil_printf("reg[%02d] = 0x%08x\n\r", i, rval);    
    // }

    tpgptr[XV_TPG_CTRL_ADDR_AP_CTRL/4] = 0x00;
    tpgptr[XV_TPG_CTRL_ADDR_GIE/4] = 0;
    tpgptr[XV_TPG_CTRL_ADDR_IER/4] = 0;
    tpgptr[XV_TPG_CTRL_ADDR_ISR/4] = 0;
    tpgptr[XV_TPG_CTRL_ADDR_HEIGHT_DATA/4] = 128;
    tpgptr[XV_TPG_CTRL_ADDR_WIDTH_DATA/4] = 70;
    tpgptr[XV_TPG_CTRL_ADDR_BCKGNDID_DATA/4] = 0x0E;
    tpgptr[XV_TPG_CTRL_ADDR_OVRLAYID_DATA/4] = 0;
    tpgptr[XV_TPG_CTRL_ADDR_MASKID_DATA/4] = 0;
    tpgptr[XV_TPG_CTRL_ADDR_MOTIONSPEED_DATA/4] = 3;
    tpgptr[XV_TPG_CTRL_ADDR_COLORFORMAT_DATA/4] = 0;
    tpgptr[XV_TPG_CTRL_ADDR_CROSSHAIRX_DATA/4] = 0;
    tpgptr[XV_TPG_CTRL_ADDR_CROSSHAIRY_DATA/4] = 0;
    tpgptr[XV_TPG_CTRL_ADDR_ZPLATEHORCONTSTART_DATA/4] = 0;
    tpgptr[XV_TPG_CTRL_ADDR_ZPLATEHORCONTDELTA_DATA/4] = 0;

    tpgptr[XV_TPG_CTRL_ADDR_AP_CTRL/4] = 0x81;


    for (int i=0; i<16; i++) {
        rval = tpgptr[i];
        xil_printf("reg[%02d] = 0x%08x\n\r", i, rval);    
    }

    while(1);
    
    return 0;
}



/*
#define XV_TPG_CTRL_ADDR_AP_CTRL                 0x00
#define XV_TPG_CTRL_ADDR_GIE                     0x04
#define XV_TPG_CTRL_ADDR_IER                     0x08
#define XV_TPG_CTRL_ADDR_ISR                     0x0c
#define XV_TPG_CTRL_ADDR_HEIGHT_DATA             0x10
#define XV_TPG_CTRL_BITS_HEIGHT_DATA             16
#define XV_TPG_CTRL_ADDR_WIDTH_DATA              0x18
#define XV_TPG_CTRL_BITS_WIDTH_DATA              16
#define XV_TPG_CTRL_ADDR_BCKGNDID_DATA           0x20
#define XV_TPG_CTRL_BITS_BCKGNDID_DATA           8
#define XV_TPG_CTRL_ADDR_OVRLAYID_DATA           0x28
#define XV_TPG_CTRL_BITS_OVRLAYID_DATA           8
#define XV_TPG_CTRL_ADDR_MASKID_DATA             0x30
#define XV_TPG_CTRL_BITS_MASKID_DATA             8
#define XV_TPG_CTRL_ADDR_MOTIONSPEED_DATA        0x38
#define XV_TPG_CTRL_BITS_MOTIONSPEED_DATA        8
#define XV_TPG_CTRL_ADDR_COLORFORMAT_DATA        0x40
#define XV_TPG_CTRL_BITS_COLORFORMAT_DATA        8
#define XV_TPG_CTRL_ADDR_CROSSHAIRX_DATA         0x48
#define XV_TPG_CTRL_BITS_CROSSHAIRX_DATA         16
#define XV_TPG_CTRL_ADDR_CROSSHAIRY_DATA         0x50
#define XV_TPG_CTRL_BITS_CROSSHAIRY_DATA         16
#define XV_TPG_CTRL_ADDR_ZPLATEHORCONTSTART_DATA 0x58
#define XV_TPG_CTRL_BITS_ZPLATEHORCONTSTART_DATA 16
#define XV_TPG_CTRL_ADDR_ZPLATEHORCONTDELTA_DATA 0x60
#define XV_TPG_CTRL_BITS_ZPLATEHORCONTDELTA_DATA 16
#define XV_TPG_CTRL_ADDR_ZPLATEVERCONTSTART_DATA 0x68
#define XV_TPG_CTRL_BITS_ZPLATEVERCONTSTART_DATA 16
#define XV_TPG_CTRL_ADDR_ZPLATEVERCONTDELTA_DATA 0x70
#define XV_TPG_CTRL_BITS_ZPLATEVERCONTDELTA_DATA 16
#define XV_TPG_CTRL_ADDR_BOXSIZE_DATA            0x78
#define XV_TPG_CTRL_BITS_BOXSIZE_DATA            16
#define XV_TPG_CTRL_ADDR_BOXCOLORR_DATA          0x80
#define XV_TPG_CTRL_BITS_BOXCOLORR_DATA          16
#define XV_TPG_CTRL_ADDR_BOXCOLORG_DATA          0x88
#define XV_TPG_CTRL_BITS_BOXCOLORG_DATA          16
#define XV_TPG_CTRL_ADDR_BOXCOLORB_DATA          0x90
#define XV_TPG_CTRL_BITS_BOXCOLORB_DATA          16
#define XV_TPG_CTRL_ADDR_ENABLEINPUT_DATA        0x98
#define XV_TPG_CTRL_BITS_ENABLEINPUT_DATA        8
#define XV_TPG_CTRL_ADDR_PASSTHRUSTARTX_DATA     0xa0
#define XV_TPG_CTRL_BITS_PASSTHRUSTARTX_DATA     16
#define XV_TPG_CTRL_ADDR_PASSTHRUSTARTY_DATA     0xa8
#define XV_TPG_CTRL_BITS_PASSTHRUSTARTY_DATA     16
#define XV_TPG_CTRL_ADDR_PASSTHRUENDX_DATA       0xb0
#define XV_TPG_CTRL_BITS_PASSTHRUENDX_DATA       16
#define XV_TPG_CTRL_ADDR_PASSTHRUENDY_DATA       0xb8
#define XV_TPG_CTRL_BITS_PASSTHRUENDY_DATA       16
#define XV_TPG_CTRL_ADDR_DPDYNAMICRANGE_DATA     0xc0
#define XV_TPG_CTRL_BITS_DPDYNAMICRANGE_DATA     8
#define XV_TPG_CTRL_ADDR_DPYUVCOEF_DATA          0xc8
#define XV_TPG_CTRL_BITS_DPYUVCOEF_DATA          8
#define XV_TPG_CTRL_ADDR_FIELDID_DATA            0xd0
#define XV_TPG_CTRL_BITS_FIELDID_DATA		 16
#define XV_TPG_CTRL_ADDR_MOTION_EN_MASK		(1<<0)
#define XV_TPG_CTRL_ADDR_BCK_MOTION_EN_DATA      0xd8
#define XV_TPG_CTRL_BITS_BCK_MOTION_EN_DATA      16
#define XV_TPG_CTRL_ADDR_FIELDID_INTERLACED_MASK (1<<0)
#define XV_TPG_CTRL_ADDR_FIELDID_INTERLACED_SHIFT 0
#define XV_TPG_CTRL_ADDR_FIELDID_POLARITY_MASK	 (1<<1)
#define XV_TPG_CTRL_ADDR_FIELDID_POLARITY_SHIFT	 1
#define XV_TPG_CTRL_ADDR_FIELDID_PASSTHR_MASK	 (1<<2)
#define XV_TPG_CTRL_ADDR_FIELDID_PASSTHR_SHIFT	 2
*/