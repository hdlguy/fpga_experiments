//
#define     FPGA_BASE_ADDRESS       0xa0000000
#define     FPGA_SIZE               0x00100000

#define     FPGA_REG_OFFSET         0x00000000

// register numbers
#define     FPGA_ID                 0
#define     FPGA_VERSION            1
#define     FPGA_LED                2 // [2:0] = PL LED[2:0], read/write

// Test Bram (rw)
#define     TEST_RAM_OFFSET         0x00010000
#define     TEST_RAM_SIZE           0x00001000 // 4KB

// XADC for temperature and voltage monitoring in the FPGA
#define     XADC_OFFSET             0x00030000
// XADC register offsets
#define     XADC_TEMP               0x200
#define     XADC_VCCINT             0x204
#define     XADC_VCCAUX             0x208
#define     XADC_VBRAM              0x218


// HW ICAP
#define     ICAP_OFFSET             0x00040000

// QSPI Interface to the configuration flash
#define     QSPI_OFFSET             0x00050000

