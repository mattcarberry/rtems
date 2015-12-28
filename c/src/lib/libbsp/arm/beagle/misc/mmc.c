/**
 * @file
 *
 * @ingroup arm_beagle
 *
 * @brief I2C support implementation.
 */

#include <rtems.h>
#include <stdio.h>

#include <bsp.h>
#include <bsp/mmc.h>
#include <libcpu/omap3.h>
#include <libcpu/am335x.h>

void mmc_init();

void mmc_init(){
//Set the pads
	//
	// Recieve enabled
	// Internal pullup/pulldown disabled
	// Pad mux is 0 for all, as the mmc0 functionality is in mode 0
	//
	printf("Writing Pad Muxes: ");
	writeb( PADCONF_RCVR_EN, AM335X_PADCONF_BASE + AM335X_CONF_MMC0_DAT3);
	writeb( PADCONF_RCVR_EN, AM335X_PADCONF_BASE + AM335X_CONF_MMC0_DAT2);
	writeb( PADCONF_RCVR_EN, AM335X_PADCONF_BASE + AM335X_CONF_MMC0_DAT1);
	writeb( PADCONF_RCVR_EN, AM335X_PADCONF_BASE + AM335X_CONF_MMC0_DAT0);
	writeb( PADCONF_RCVR_EN, AM335X_PADCONF_BASE + AM335X_CONF_MMC0_CLK);
	writeb( PADCONF_RCVR_EN, AM335X_PADCONF_BASE + AM335X_CONF_MMC0_CMD);
	printf("Done!\n");

//Initialize Clocks
	//Enabled SD OCP clock
	//Enabled CLKADPI clock
	printf("Enabling MMC0 Clock: ");
	writeb( CM_MODULEMODE_ENABLE, CM_PER_BASE + CM_PER_MMC0_CLKCTRL); 
	printf("Done!\n");
	//Enable the clockdivisor
	printf("Enabling 32Khz Clock Divisor: ");
	writeb( CM_MODULEMODE_ENABLE, CM_PER_BASE + CM_PER_CLKDIV32K_CLKCTRL);
	printf("Done!\n");

//Software reset of controller
	// Set SD_SYSCONFIG[1] SOFTRESET bit to 0x1
	//Must access as 32 bits
	
	printf("Resetting MMC Controller: ");
	mmio_set(AM335X_MMC0_BASE + SD_SYSCONFIG, 0x1); 	
	// Read the SD_SYSSTATUS[0] RESETDONE bit until RESETDONE=0x1
	uint32_t status;
	uint32_t one = 0x1;
	do{
		status = mmio_read(AM335X_MMC0_BASE+SD_SYSSTATUS);	
		printf(".");
	} while( (status & one) != one);
	printf("Done!\n");
	
	//Set module's hardware capabilities
		//SD_CAPA fields
	printf("Setting SD Capability Fields: ");
	mmio_set(AM335X_MMC0_BASE + SD_CAPA, SD_CAPA_VS33_EN);
	printf("Done!\n");
	//Set modules Idle and wakeup modes

	//MMC Host and Bus Configuration
	
	//Write SD_CON
	printf("Setting Data and Command Transfer: ");
	mmio_clear(AM335X_MMC0_BASE + SD_CON, SD_CON_CEATA_EN | SD_CON_DW8_EN | SD_CON_OD_EN) 
	printf("Done!\n");

	//Write SD_HCTL 
	printf("Setting card voltage 3.3V: ");
	mmio_set(AM335X_MMC0_BASE + SD_HCTL, SD_HCTL_SDVS_3V3);
	printf("Done!\n");
	
	printf("Setting bus width 1-bit: ");
	mmio_clear(AM335X_MMC0_BASE + SD_HCTL, SD_HCTL_DTW_4BIT);
	printf("Done!\n");

	printf("Turning on SD Bus Power: ");
	mmio_set(AM335X_MMC0_BASE + SD_HCTL, SD_HCTL_SDBP_EN);
	status = mmio_read(AM335X_MMC0_BASE+SD_HCTL);	
	if(status & SD_HCTL_SDBP_EN == 0x0){ 
		printf("Failure");
	} else {
		printf("Done!\n");
	}

	//Set SD_SYSCTL ICE to enable internal clock
	printf("Enabling internal clock: ");
	mmio_set(AM335X_MMC0_BASE + SD_SYSCTL, SD_SYSCTL_ICE_EN);
	printf("Done!\n");

	printf("Setting Clock Divisor to 3 for initialization: ");
	mmio_clear(AM335X_MMC0_BASE + SD_SYSCTL, SD_SYSCTL_CLKD_3FF);
	mmio_set(AM335X_MMC0_BASE + SD_SYSCTL, SD_SYSCTL_CLKD_3);
	printf("Done!\n");

	// Reading the internal clock status until it's stable
	do{
		status = mmio_read(AM335X_MMC0_BASE+SD_SYSCTL);	
		printf(".");
	} while((status & SD_SYSCTL_ICS_ON) != SD_SYSCTL_ICS_ON );
	printf("Done!\n");

	//Configure the idle mode behavior CLOCKACTIVITY, SIDLEMODE, AUTOIDLE
	mmio_clear(AMM335X_MMC0_BASE + SD_SYSCONFIG, SD_SYSCONFIG_SIDLEMODE_MASK);
	mmio_set(AM335X_MMC0_BASE + SD_SYSCONFIG, 
			SD_SYSCONFIG_CLOCKACTIVITY_INTERFACECLK_EN |
			SD_SYSCONFIG_CLOCKACTIVITY_FUNCTIONALCLK_EN |
			SD_SYSCONFIG_SIDLEMODE_NOIDLE);


}

