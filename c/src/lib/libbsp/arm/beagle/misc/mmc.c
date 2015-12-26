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
	printf("Done!\n");
	
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
	printf("Setting SD Capability Fields");
	mmio_set(AM335X_MMC0_BASE + SD_CAPA, SD_CAPA_VS33_EN);
	printf("Done!\n");
	//Set modules Idle and wakeup modes

	//MMC Host and Bus Configuration
}
