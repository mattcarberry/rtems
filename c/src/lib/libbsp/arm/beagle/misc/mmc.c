#include <bsp.h>
#include <libcpu/omap3.h>
#include <libcpu/am335x.h>

void mmc_init(){
//Set the pads
	//
	// Recieve enabled
	// Internal pullup/pulldown disabled
	// Pad mux is 0 for all, as the mmc0 functionality is in mode 0
	//
	writeb( PADCONF_RCVR_EN, AM335X_PADCONF_BASE + AM335X_CONF_MMC0_DAT3);
	writeb( PADCONF_RCVR_EN, AM335X_PADCONF_BASE + AM335X_CONF_MMC0_DAT2);
	writeb( PADCONF_RCVR_EN, AM335X_PADCONF_BASE + AM335X_CONF_MMC0_DAT1);
	writeb( PADCONF_RCVR_EN, AM335X_PADCONF_BASE + AM335X_CONF_MMC0_DAT0);
	writeb( PADCONF_RCVR_EN, AM335X_PADCONF_BASE + AM335X_CONF_MMC0_CLK);
	writeb( PADCONF_RCVR_EN, AM335X_PADCONF_BASE + AM335X_CONF_MMC0_CMD);

//Initialize Clocks
	//Enabled SD OCP clock
	//Enabled CLKADPI clock
	writeb( CM_MODULEMODE_ENABLE, CM_PER_MMC0_CLKCTRL); 
	//Enable the clockdivisor
	writeb( CM_MODULEMODE_ENABLE, CM_PER_CLKDIV32K_CLKCTRL);

//Software reset of controller
	// Set SD_SYSCONFIG[1] SOFTRESET bit to 0x1
	//Must access as 32 bits
	mmio_set(AM335X_MMC0_BASE + SD_SYSCONFIG, 1); 
	
	// Read the SD_SYSSTATUS[0] RESETDONE bit until RESETDONE=0x1
	do{
		uint32_t status = mmio_read(AM335X_MMC0_BASE+SD_SYSSTATUS);	
	} while( (status & 0x1) != 0x1);
	
	//Set module's hardware capabilities
		//SD_CAPA fields
		mmio_set(AM335X_MMC0_BASE + SD_CAPA, SD_CAPA_VS33_EN);

	//Set modules Idle and wakeup modes

	//MMC Host and Bus Configuration
	
}
