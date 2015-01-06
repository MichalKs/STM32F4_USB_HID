/**
 * @file    usbd_usr.c
 * @brief   User callback for USB library
 * @date    6 sty 2015
 * @author  Michal Ksiezopolski
 *
 *
 * @verbatim
 * Copyright (c) 2015 Michal Ksiezopolski.
 * All rights reserved. This program and the
 * accompanying materials are made available
 * under the terms of the GNU Public License
 * v3.0 which accompanies this distribution,
 * and is available at
 * http://www.gnu.org/licenses/gpl.html
 * @endverbatim
 */

#include <usbd_usr.h>
#include <stdio.h>

#define DEBUG

#ifdef DEBUG
#define print(str, args...) printf(""str"%s",##args,"")
#define println(str, args...) printf("USB_USR--> "str"%s",##args,"\r\n")
#else
#define print(str, args...) (void)0
#define println(str, args...) (void)0
#endif

/**
 * @brief User callbacks for USB events
 */
USBD_Usr_cb_TypeDef USR_cb = {
  USBD_USR_Init,
  USBD_USR_DeviceReset,
  USBD_USR_DeviceConfigured,
  USBD_USR_DeviceSuspended,
  USBD_USR_DeviceResumed,
  USBD_USR_DeviceConnected,
  USBD_USR_DeviceDisconnected,    
};
/**
 *
 */
void USBD_USR_Init(void) {

  println("USR Init");
}
/**
 *
 * @param speed
 */
void USBD_USR_DeviceReset(uint8_t speed) {
  println("Device reset");
}
/**
 *
 */
void USBD_USR_DeviceConfigured (void) {
  println("Device configured");
}
/**
 *
 */
void USBD_USR_DeviceSuspended(void) {
  println("Device suspended");
}
/**
 *
 */
void USBD_USR_DeviceResumed(void) {
  println("Device resumed");
}
/**
 *
 */
void USBD_USR_DeviceConnected (void) {
  println("Device connected");
}
/**
 *
 */
void USBD_USR_DeviceDisconnected (void) {
  println("Device disconnected");
}
