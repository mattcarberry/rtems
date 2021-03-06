/*
 *  Instantiate the clock driver shell.
 *
 *  Since there is no clock source on the simulator, we fake
 *  it with a special IDLE task.
 */

#include <rtems.h>
#include <rtems/score/percpu.h>
#include <rtems/score/threaddispatch.h>

#define CLOCK_VECTOR 0

#define Clock_driver_support_at_tick()

#define Clock_driver_support_install_isr( _new, _old ) \
  do { _old = 0; } while(0)

volatile bool clock_driver_enabled;

#define Clock_driver_support_initialize_hardware() \
  do { \
    clock_driver_enabled = true; \
  } while (0)

#define Clock_driver_support_shutdown_hardware() \
  do { \
    clock_driver_enabled = false; \
  } while (0)

#define CLOCK_DRIVER_USE_DUMMY_TIMECOUNTER

#include "clockdrv_shell.h"

/*
 * If this is defined, then the BSP has defined a delay of some sort so
 * time passage appears somewhat correctly. Otherwise, it runs extremely
 * fast with no delays.
 */
#ifndef BSP_CLOCK_DRIVER_DELAY
#define BSP_CLOCK_DRIVER_DELAY()
#endif

/*
 *  Since there is no interrupt on this simulator, let's just
 *  fake time passing.  This will not let preemption from an
 *  interrupt work but is enough for many tests.
 */
Thread clock_driver_sim_idle_body(
  uintptr_t   ignored
)
{
  for( ; ; ) {
    if ( clock_driver_enabled ) {
      _Thread_Disable_dispatch();
      _ISR_Nest_level++;
	rtems_clock_tick();
      _ISR_Nest_level--;
      _Thread_Enable_dispatch();
      BSP_CLOCK_DRIVER_DELAY();
    }
  }
  return 0;   /* to avoid warning */
}
