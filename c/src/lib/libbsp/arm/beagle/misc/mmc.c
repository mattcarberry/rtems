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


int mmc_init(uint8_t mmc_number);
void mmc_pad_conf(uint8_t mmc_number);

void mmc_set_reg_debug(uint32_t addr, uint32_t mask, uint32_t value);

void mmc_pad_conf(uint8_t mmc_number){
	printf("Writing Pad Muxes for MMC%u: ", mmc_number);
	if(mmc_number == 0) {
		mmio_set( AM335X_PADCONF_BASE + AM335X_CONF_MMC0_DAT3, PADCONF_RCVR_EN);
		mmio_set( AM335X_PADCONF_BASE + AM335X_CONF_MMC0_DAT2, PADCONF_RCVR_EN);
		mmio_set( AM335X_PADCONF_BASE + AM335X_CONF_MMC0_DAT1, PADCONF_RCVR_EN);
		mmio_set( AM335X_PADCONF_BASE + AM335X_CONF_MMC0_DAT0, PADCONF_RCVR_EN);
		mmio_set( AM335X_PADCONF_BASE + AM335X_CONF_MMC0_CLK, PADCONF_RCVR_EN);
		mmio_set( AM335X_PADCONF_BASE + AM335X_CONF_MMC0_CMD, PADCONF_RCVR_EN);
	} else if(mmc_number == 1){
		
		set32(AM335X_PADCONF_BASE + AM335X_CONF_GPMC_AD3, 
			   PADCONF_MODE_MASK | PADCONF_PUDEN_MASK, 
			   PADCONF_RCVR_EN | PADCONF_TYPESEL_PULLUP | PADCONF_MODE_ONE);
		
		set32(AM335X_PADCONF_BASE + AM335X_CONF_GPMC_AD2, 
			   PADCONF_MODE_MASK | PADCONF_PUDEN_MASK, 
			   PADCONF_RCVR_EN | PADCONF_TYPESEL_PULLUP | PADCONF_MODE_ONE);

		set32(AM335X_PADCONF_BASE + AM335X_CONF_GPMC_AD1, 
			   PADCONF_MODE_MASK | PADCONF_PUDEN_MASK, 
			   PADCONF_RCVR_EN | PADCONF_TYPESEL_PULLUP | PADCONF_MODE_ONE);

		set32(AM335X_PADCONF_BASE + AM335X_CONF_GPMC_AD0, 
			   PADCONF_MODE_MASK | PADCONF_PUDEN_MASK, 
			   PADCONF_RCVR_EN | PADCONF_TYPESEL_PULLUP | PADCONF_MODE_ONE);

		set32(AM335X_PADCONF_BASE + AM335X_CONF_GPMC_CSN1, 
			   PADCONF_MODE_MASK | PADCONF_PUDEN_MASK, 
			   PADCONF_RCVR_EN | PADCONF_TYPESEL_PULLUP | PADCONF_MODE_TWO);

		set32(AM335X_PADCONF_BASE + AM335X_CONF_GPMC_CSN2, 
			   PADCONF_MODE_MASK | PADCONF_PUDEN_MASK, 
			   PADCONF_RCVR_EN | PADCONF_TYPESEL_PULLUP | PADCONF_MODE_TWO);
	}
}


int mmc_init(uint8_t mmc_number){
	if((mmc_number > 2) | (mmc_number < 0)) return 1;	

	uint32_t mmc_clkctrl, base;
	switch(mmc_number){
		case 0:
			mmc_clkctrl = CM_PER_BASE + CM_PER_MMC0_CLKCTRL;
			base = AM335X_MMC0_BASE;
			break;
		case 1:
			mmc_clkctrl = CM_PER_BASE + CM_PER_MMC1_CLKCTRL;
			base = AM335X_MMC1_BASE;
			break;
		case 2:
			mmc_clkctrl = CM_PER_BASE + CM_PER_MMC1_CLKCTRL;
			base = AM335X_MMC2_BASE;
			break;
	}

	//Set the pads
	mmc_pad_conf(mmc_number);

	printf("Enabling MMC1 Clock: ");
	set32(mmc_clkctrl, CM_PER_MODULEMODE_MASK, CM_PER_MODULEMODE_ENABLE); 
	printf("\tCM_PER_MMC1_CLKCTRL addr:%lx value: %lx\n", (uint32_t) mmc_clkctrl, (uint32_t) read32(mmc_clkctrl));
	printf("Done!\n");
	
	printf("Enabling 32Khz Clock Divisor: "); //For input debounce 
	set32(CM_PER_BASE + CM_PER_CLKDIV32K_CLKCTRL, CM_PER_MODULEMODE_MASK, CM_PER_MODULEMODE_ENABLE);
	printf("\tCM_PER_32DIV_CLKCTRL addr:%lx value: %lx\n", (uint32_t) CM_PER_BASE + CM_PER_CLKDIV32K_CLKCTRL, (uint32_t) read32(CM_PER_BASE + CM_PER_CLKDIV32K_CLKCTRL));
	printf("Done!\n");

	printf("Resetting MMC Controller: ");
	set32(base + SD_SYSCONFIG, (1 << 1), (1 << 1)); 	
	// Read the SD_SYSSTATUS[0] RESETDONE bit until RESETDONE=0x1
	uint32_t status;
	do{
		status =  (uint32_t) read32(base+SD_SYSSTATUS);	
		printf(".");
	} while( (status & 1) != 1);
	printf("\tSD_SYSSTATUS addr:%lx value: %lx\n", (uint32_t) base+SD_SYSSTATUS, (uint32_t) read32(base+SD_SYSSTATUS));
	printf("Done!\n");
	
	//The setup for the beaglebonewhite is 3.3V for MMC0
	printf("Setting SD Capability Fields: ");
	set32(base + SD_CAPA, SD_CAPA_MASK, SD_CAPA_VS33_EN);
	printf("\tSD_CAPA addr:%lx value: %lx\n", (uint32_t) base+SD_CAPA, (uint32_t) read32(base+SD_CAPA));
	printf("Done!\n");
	


	/* MMC Host and Bus Configuration */
	//Neither setting CE-ATA, 8-bit data width, or open drain. Reset values are ok
	printf("Done!\n");

	printf("Setting card voltage 3.3V: ");
	mmio_set(base + SD_HCTL, SD_HCTL_SDVS_3V3); //Only SD cards
	printf("\tSD_HCTL addr:%lx value: %lx\n", (uint32_t) base+SD_HCTL, (uint32_t) read32(base+SD_HCTL));
	printf("Done!\n");
	
	printf("Setting bus width 4-bit for SD: ");
	mmio_set(base + SD_HCTL, SD_HCTL_DTW_4BIT);
	printf("\tSD_HCTL addr:%lx value: %lx\n", (uint32_t) base+SD_HCTL, (uint32_t) read32(base+SD_HCTL));
	printf("Done!\n");

	printf("Turning on SD Bus Power: ");
	mmio_set(base + SD_HCTL, SD_HCTL_SDBP_EN);
	status =  (uint32_t) read32(base+SD_HCTL);	
	if((status & SD_HCTL_SDBP_EN) == 0x0){ 
		printf("Failure\n");
	} else {
		printf("Done!\n");
	}

	//Set SD_SYSCTL ICE to enable internal clock
	printf("Enabling internal clock: ");
	mmio_set(base + SD_SYSCTL, SD_SYSCTL_ICE_EN);
	printf("\tSD_SYSCTL addr:%lx value: %lx\n", (uint32_t) base+SD_SYSCTL, (uint32_t) read32(base+SD_SYSCTL));
	printf("Done!\n");

	printf("Setting Clock Divisor to 3 for initialization: ");
	mmio_set(base + SD_SYSCTL, SD_SYSCTL_CLKD_1023);
	printf("\tSD_SYSCTL addr:%lx value: %lx\n", (uint32_t) base+SD_SYSCTL, (uint32_t) read32(base+SD_SYSCTL));
	printf("Done!\n");
	
	printf("Waiting until clock is stable: ");
	// Reading the internal clock status until it's stable
	do{
		status =  (uint32_t) read32(base+SD_SYSCTL);	
		printf(".");
	} while((status & SD_SYSCTL_ICS_ON) != SD_SYSCTL_ICS_ON );
	printf("\tSD_SYSCTL addr:%lx value: %lx\n", (uint32_t) base+SD_SYSCTL, (uint32_t) read32(base+SD_SYSCTL));
	printf("Done!\n");

	//Enabling clock output
	set32(base + SD_SYSCTL, (1 << 2), (1 << 2));
	printf("\tSD_SYSCTL addr:%lx value: %lx\n", (uint32_t) base+SD_SYSCTL, (uint32_t) read32(base+SD_SYSCTL));
	
	//Configure the idle mode behavior CLOCKACTIVITY, SIDLEMODE, AUTOIDLE
	printf("Configuring idle mode behavior: ");
	mmio_set(base + SD_SYSCONFIG, 
			SD_SYSCONFIG_CLOCKACTIVITY_INTERFACECLK_EN |
			SD_SYSCONFIG_CLOCKACTIVITY_FUNCTIONALCLK_EN |
			SD_SYSCONFIG_SIDLEMODE_NOIDLE);
	printf("SD_SYSCONFIG addr:%lx value: %lx\n", (uint32_t) base+SD_SYSCONFIG, (uint32_t) read32(base+SD_SYSCONFIG));
	printf("Done!\n");

	// Send initialization stream
	mmio_set(base + SD_CON, (1 << 1));
	printf("SD_CON addr:%lx value: %lx\n", (uint32_t) base+SD_CON, (uint32_t) read32(base+SD_CON));
	
	write32(base + SD_CMD, (uint32_t) 0x0);

	return 0;
}
