#ifndef LIBBSP_ARM_BEAGLE_MMC_H
#define LIBBSP_ARM_BEAGLE_MMC_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Pad configuration */
#define PADCONF_RCVR_EN (1 << 5)

/* Clock Module Prepherial Registers */
#define CM_PER_BASE 0x44E00000
#define CM_PER_MMC0_CLKCTRL 0x3C
#define CM_PER_MMC1_CLKCTRL 0xF4
#define CM_PER_MMC2_CLKCTRL 0xF8
#define CM_PER_CLKDIV32K_CLKCTRL 0x14c

// For Turning on and off clocks in these registers
#define CM_PER_CLK_ENABLE	0x2
#define CM_PER_DISABLE		0x0


/* MULTIMEDIA_CARD Registers */
#define AM335X_MMC0_BASE 0x48060000
#define AM335X_MMC1_BASE 0x481D8000
#define AM335X_MMC2_BASE 0x47810000
//Offsets
#define SD_SYSCONFIG	0x110
#define SD_SYSSTATUS 	0x114
#define SD_CSRE		0x124
#define SD_SYSTEST	0x128
#define SD_CON		0x12c
#define SD_PWCNT	0x130
#define SD_SDMASA	0x200
#define SD_BLK		0x204
#define SD_ARG		0x208
#define SD_CMD		0x20c
#define SD_RSP10	0x210
#define SD_RSP32	0x214
#define SD_RSP54	0x218
#define SD_RSP76	0x21c
#define SD_DATA		0x220
#define	SD_PSTATE	0x224
#define SD_HCTL		0x228
#define SD_SYSCTL	0x22c
#define SD_STAT		0x230
#define SD_IE		0x234
#define SD_ISE		0x238
#define SD_AC12		0x23c
#define SD_CAPA		0x240
#define SD_CUR_CAPA	0x248
#define SD_FE		0x250
#define SD_ADMAES	0x254
#define SD_ADMASAL	0x258
#define SD_ADMASAH	0x25c
#define SD_REV		0x2FC

//SD_SYSCONFIG
#define SD_SYSCONFIG_CLOCKACTIVITY_INTERFACECLK_EN (1 << 8) //Clock activity during wakeup mode
#define SD_SYSCONFIG_CLOCKACTIVITY_FUNCTIONALCLK_EN (1 << 9)
#define SD_SYSCONFIG_SIDLEMODE_NOIDLE (1 << 3)
#define SD_SYSCONFIG_SIDLEMODE_MASK (3 << 3)
#define SD_SYSCONFIG_AUTOGATING_EN (1 << 0)

//SD_CON
#define SD_CON_SLAVE_DMA_EDGE (0 << 21)
#define SD_CON_SLAVE_DMA_LEVEL (1 << 21)
#define SD_CON_CEATA_EN (1 << 12)
#define SD_CON_DW8_EN (1 << 5)
#define SD_CON_OD_EN 1

//SD_HCTL
#define SD_HCTL_SDVS_1V8 (5 << 9)
#define SD_HCTL_SDVS_3V0 (6 << 9)
#define SD_HCTL_SDVS_3V3 (7 << 9)
#define SD_HCTL_SDBP_EN (1 << 8)
#define SD_HCTL_DTW_1BIT (0 << 1)
#define SD_HCTL_DTW_4BIT (1 << 1)

//SD_SYSCTL
#define SD_SYSCTL_ICE_EN (1 << 0)
#define SD_SYSCTL_ICS_ON (1 << 1)
#define SD_SYSCTL_CLKD_1 (1 << 6)
#define SD_SYSCTL_CLKD_2 (2 << 6)
#define SD_SYSCTL_CLKD_3 (3 << 6)
#define SD_SYSCTL_CLKD_3FF (0x3FF << 6)

//SD_CAPA 
#define SD_CAPA_BUS_64BIT_EN (1 << 28) //64-bit bus support
#define SD_CAPA_VS18_EN (1 << 26) // Voltage Support 1.8V
#define SD_CAPA_VS30_EN (1 << 25) // Voltage Support 3.0V
#define SD_CAPA_VS33_EN (1 << 24) // Voltage Support 3.3V
#define SD_CAPA_SRS	(1 << 23) // Suspend/Resume SUpport
#define SD_CAPA_DS	(1 << 22) // DMA Support
#define SD_CAPA_HSS	(1 << 21) // High Speed Support
#define SD_CAPA_AD2S (1 << 19) // ADMA2 Support
#define SD_CAPA_MBL_512	(0 << 16) // Minimum block length in bytes
#define SD_CAPA_MBL_1024 (1 << 16)	
#define SD_CAPA_MBL_2048 (2 << 16)


#endif /* LIBBSP_ARM_BEAGLE_MMC_H */
