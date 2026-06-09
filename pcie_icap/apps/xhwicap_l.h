#ifndef XHWICAP_L_H_ /* prevent circular inclusions */
#define XHWICAP_L_H_ /* by using protection macros */


#define XHI_GIER_OFFSET		0x1C  /**< Device Global Interrupt Enable Reg */
#define XHI_IPISR_OFFSET	0x20  /**< Interrupt Status Register */
#define XHI_IPIER_OFFSET	0x28  /**< Interrupt Enable Register */
#define XHI_WF_OFFSET		0x100 /**< Write FIFO */
#define XHI_RF_OFFSET		0x104 /**< Read FIFO */
#define XHI_SZ_OFFSET		0x108 /**< Size Register */
#define XHI_CR_OFFSET		0x10C /**< Control Register */
#define XHI_SR_OFFSET		0x110 /**< Status Register */
#define XHI_WFV_OFFSET		0x114 /**< Write FIFO Vacancy Register */
#define XHI_RFO_OFFSET		0x118 /**< Read FIFO Occupancy Register */

#define XHI_GIER_GIE_MASK      0x80000000 /**< Global Interrupt enable Mask */

#define XHI_IPIXR_RFULL_MASK    0x00000008 /**< Read FIFO Full */
#define XHI_IPIXR_WEMPTY_MASK   0x00000004 /**< Write FIFO Empty */
#define XHI_IPIXR_RDP_MASK      0x00000002 /**< Read FIFO half full */
#define XHI_IPIXR_WRP_MASK      0x00000001 /**< Write FIFO half full */
#define XHI_IPIXR_ALL_MASK      0x0000000F /**< Mask of all interrupts */

#define XHI_CR_SW_ABORT_MASK	0x00000010 /**< Abort current ICAP Read/Write */
#define XHI_CR_SW_RESET_MASK	0x00000008 /**< SW Reset Mask */
#define XHI_CR_FIFO_CLR_MASK	0x00000004 /**< FIFO Clear Mask */
#define XHI_CR_READ_MASK	0x00000002 /**< Read from ICAP to FIFO */
#define XHI_CR_WRITE_MASK	0x00000001 /**< Write from FIFO to ICAP */

#define XHI_SR_CFGERR_N_MASK	0x00000100 /**< Config Error Mask */
#define XHI_SR_DALIGN_MASK	0x00000080 /**< Data Alignment Mask */
#define XHI_SR_RIP_MASK		0x00000040 /**< Read back Mask */
#define XHI_SR_IN_ABORT_N_MASK	0x00000020 /**< Select Map Abort Mask */
#define XHI_SR_DONE_MASK	0x00000001 /**< Done bit Mask */
#define XHI_SR_EOS_MASK 	0x00000004 /**< EOS bit Mask */

#endif         /* end of protection macro */
