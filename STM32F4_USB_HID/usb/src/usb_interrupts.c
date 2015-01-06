/**
 * @file    usb_interrupts.c
 * @brief
 * @date    4 sty 2015
 * @author  Michal Ksiezopolski
 *
 *
 * @verbatim
 * Copyright (c) 2014 Michal Ksiezopolski.
 * All rights reserved. This program and the
 * accompanying materials are made available
 * under the terms of the GNU Public License
 * v3.0 which accompanies this distribution,
 * and is available at
 * http://www.gnu.org/licenses/gpl.html
 * @endverbatim
 */

#include <stm32f4xx.h>
#include "usb_core.h"
#include "usbd_core.h"
#include "usbd_cdc_core.h"
#include <stdio.h>

extern USB_OTG_CORE_HANDLE USB_OTG_dev;

void OTG_FS_WKUP_IRQHandler(void) {

  if(USB_OTG_dev.cfg.low_power) {
    *(uint32_t *)(0xE000ED10) &= 0xFFFFFFF9 ;
    SystemInit();
    USB_OTG_UngateClock(&USB_OTG_dev);
    printf("Woke up in OTG FS interrupt\r\n");
  }
  printf("Woke up in OTG FS interrupt\r\n");
  EXTI_ClearITPendingBit(EXTI_Line18);
}

void OTG_FS_IRQHandler(void) {
  USBD_OTG_ISR_Handler (&USB_OTG_dev);
}
