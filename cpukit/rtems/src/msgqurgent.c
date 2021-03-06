/**
 *  @file
 *
 *  @brief RTEMS Urgent Message Queue
 *  @ingroup ClassicMessageQueue
 */

/*
 *  COPYRIGHT (c) 1989-2014.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/chain.h>
#include <rtems/score/isr.h>
#include <rtems/score/coremsgimpl.h>
#include <rtems/score/thread.h>
#include <rtems/score/wkspace.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/attrimpl.h>
#include <rtems/rtems/messageimpl.h>
#include <rtems/rtems/options.h>
#include <rtems/rtems/support.h>

#if defined(RTEMS_MULTIPROCESSING)
#define MESSAGE_QUEUE_MP_HANDLER _Message_queue_Core_message_queue_mp_support
#else
#define MESSAGE_QUEUE_MP_HANDLER NULL
#endif

rtems_status_code rtems_message_queue_urgent(
  rtems_id    id,
  const void *buffer,
  size_t      size
)
{
  Message_queue_Control           *the_message_queue;
  Objects_Locations                location;
  CORE_message_queue_Status        status;
  ISR_lock_Context                 lock_context;

  if ( !buffer )
    return RTEMS_INVALID_ADDRESS;

  the_message_queue = _Message_queue_Get_interrupt_disable(
    id,
    &location,
    &lock_context
  );
  switch ( location ) {

    case OBJECTS_LOCAL:
      status = _CORE_message_queue_Urgent(
        &the_message_queue->message_queue,
        buffer,
        size,
        id,
        MESSAGE_QUEUE_MP_HANDLER,
        false,   /* sender does not block */
        0,       /* no timeout */
        &lock_context
      );

      /*
       *  Since this API does not allow for blocking sends, we can directly
       *  return the returned status.
       */

      return _Message_queue_Translate_core_message_queue_return_code(status);

#if defined(RTEMS_MULTIPROCESSING)
    case OBJECTS_REMOTE:
      return _Message_queue_MP_Send_request_packet(
        MESSAGE_QUEUE_MP_URGENT_REQUEST,
        id,
        buffer,
        &size,
        0,                               /* option_set */
        MPCI_DEFAULT_TIMEOUT
      );
#endif

    case OBJECTS_ERROR:
      break;
  }

  return RTEMS_INVALID_ID;
}
