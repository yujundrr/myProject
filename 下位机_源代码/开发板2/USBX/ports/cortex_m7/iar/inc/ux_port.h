/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */ 
/** USBX Component                                                        */ 
/**                                                                       */
/**   Port Specific                                                       */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/ 
/*                                                                        */ 
/*  PORT SPECIFIC C INFORMATION                            RELEASE        */ 
/*                                                                        */ 
/*    ux_port.h                                         Cortex-M7/IAR     */ 
/*                                                           6.1          */
/*                                                                        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */ 
/*    This file contains data type definitions that make USBX function    */ 
/*    identically on a variety of different processor architectures.      */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  09-30-2020     Chaoqiong Xiao           Initial Version 6.1           */
/*                                                                        */
/**************************************************************************/

#ifndef UX_PORT_H
#define UX_PORT_H


/* Determine if the optional USBX user define file should be used.  */

#ifdef UX_INCLUDE_USER_DEFINE_FILE


/* Yes, include the user defines in ux_user.h. The defines in this file may 
   alternately be defined on the command line.  */

#include "ux_user.h"
#endif


/* Include library header files.  */

#include <stdio.h>
#include <string.h>


/* CPU definition for X86 systems without preemptive timer function.
   This will make USBX uses the controller for the timer. */

#undef THREADX_X86_NO_PTIMER


/* For X86 systems, the define #define UX_USE_IO_INSTRUCTIONS should be used.  */


/* Define additional generic USBX types.  */

typedef long                        SLONG;


/*  Generic USBX Project constants follow.  */

#ifndef UX_PERIODIC_RATE
#define UX_PERIODIC_RATE                                    100
#endif

#ifndef UX_MAX_CLASS_DRIVER
#define UX_MAX_CLASS_DRIVER                                 8
#endif

#ifndef UX_MAX_SLAVE_CLASS_DRIVER
#define UX_MAX_SLAVE_CLASS_DRIVER                           4
#endif

#ifndef UX_MAX_HCD
#define UX_MAX_HCD                                          2
#endif

#ifndef UX_MAX_DEVICES
#define UX_MAX_DEVICES                                      8
#endif

#ifndef UX_MAX_ED
#define UX_MAX_ED                                           80
#endif

#ifndef UX_MAX_TD
#define UX_MAX_TD                                           32
#endif

#ifndef UX_MAX_ISO_TD
#define UX_MAX_ISO_TD                                       16
#endif

#ifndef UX_HOST_ENUM_THREAD_STACK_SIZE
#define UX_HOST_ENUM_THREAD_STACK_SIZE                      (2*1024)
#endif

#ifndef UX_THREAD_STACK_SIZE
#define UX_THREAD_STACK_SIZE                                (1*1024)
#endif

#ifndef UX_THREAD_PRIORITY_ENUM
#define UX_THREAD_PRIORITY_ENUM                             20
#endif

#ifndef UX_THREAD_PRIORITY_CLASS
#define UX_THREAD_PRIORITY_CLASS                            4
#endif

#ifndef UX_THREAD_PRIORITY_KEYBOARD
#define UX_THREAD_PRIORITY_KEYBOARD                         20
#endif

#ifndef UX_THREAD_PRIORITY_HCD
#define UX_THREAD_PRIORITY_HCD                              2
#endif

#ifndef UX_THREAD_PRIORITY_DCD
#define UX_THREAD_PRIORITY_DCD                              2
#endif

#ifndef UX_NO_TIME_SLICE
#define UX_NO_TIME_SLICE                                    0
#endif

#ifndef UX_MAX_SLAVE_LUN
#define UX_MAX_SLAVE_LUN                                    2
#endif

#ifndef UX_MAX_HOST_LUN
#define UX_MAX_HOST_LUN                                     2
#endif

#ifndef UX_HOST_CLASS_STORAGE_MAX_MEDIA
#define UX_HOST_CLASS_STORAGE_MAX_MEDIA                     2
#endif

#ifndef UX_SLAVE_REQUEST_CONTROL_MAX_LENGTH
#define UX_SLAVE_REQUEST_CONTROL_MAX_LENGTH                 256
#endif


#ifndef UX_SLAVE_REQUEST_DATA_MAX_LENGTH
#define UX_SLAVE_REQUEST_DATA_MAX_LENGTH                    4096
#endif

#ifndef UX_USE_IO_INSTRUCTIONS

/* Don't use IO instructions if this define is not set.  Default to memory mapped.  */

#define inpb(a)                                            *((UCHAR *)  (a))
#define inpw(a)                                            *((USHORT *) (a))
#define inpl(a)                                            *((ULONG *)  (a))
#define outpb(a, b)                                        *((UCHAR *)  (a)) =  ((UCHAR)  (b))
#define outpw(a, b)                                        *((USHORT *) (a)) =  ((USHORT) (b))
#define outpl(a, b)                                        *((ULONG *)  (a)) =  ((ULONG)  (b))
#else


/* Define simple prototypes for non-memory mapped hardware access.  */

UCHAR   inpb(ULONG);
USHORT  inpw(ULONG);
ULONG   inpl(ULONG);

VOID    outpb(ULONG,UCHAR);
VOID    outpw(ULONG,USHORT);
VOID    outpl(ULONG,ULONG);

#endif
/* Define local delay function for board specific bsps.  */
#ifdef TI_AM335
    #define UX_BSP_SPECIFIC_DELAY_FUNCTION
#endif


/* Define interrupt lockout constructs to protect the memory allocation/release which could happen
   under ISR in the device stack.  */

#define UX_INT_SAVE_AREA        unsigned int  old_interrupt_posture;
#define UX_DISABLE_INTS         old_interrupt_posture =  tx_interrupt_control(TX_INT_DISABLE);
#define UX_RESTORE_INTS         tx_interrupt_control(old_interrupt_posture);


/* Define the version ID of USBX.  This may be utilized by the application.  */

#ifdef  UX_SYSTEM_INIT
CHAR                            _ux_version_id[] = 
                                    "Copyright (c) Microsoft Corporation. All rights reserved. * USBX Cortex-M7/IAR Version 6.1 *";
#else
extern  CHAR                    _ux_version_id[];
#endif

#endif

