/**
 * @file    main.c
 * @brief   STM32F4 USB CDC test
 * @date    9 kwi 2014
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

#include <stdio.h>
#include <string.h>

#include <timers.h>
#include <led.h>
#include <comm.h>
#include <keys.h>

// USB includes
#include <usbd_usr.h>
#include <usb_conf.h>
#include <usbd_desc.h>
#include <usbd_hid_core.h>

#define SYSTICK_FREQ 1000 ///< Frequency of the SysTick set at 1kHz.
#define COMM_BAUD_RATE 115200UL ///< Baud rate for communication with PC

void softTimerCallback(void);
void usbSoftTimerCallback(void);

#define DEBUG

#ifdef DEBUG
#define print(str, args...) printf(""str"%s",##args,"")
#define println(str, args...) printf("MAIN--> "str"%s",##args,"\r\n")
#else
#define print(str, args...) (void)0
#define println(str, args...) (void)0
#endif

__ALIGN_BEGIN USB_OTG_CORE_HANDLE USB_OTG_dev __ALIGN_END; ///< USB device handle

static uint8_t key;

/**
 * @brief Main function
 * @return None
 */
int main(void) {

  COMM_Init(COMM_BAUD_RATE); // initialize communication with PC
  println("Starting program"); // Print a string to terminal

  TIMER_Init(SYSTICK_FREQ); // Initialize timer

  // Add a soft timer with callback running every 1000ms
  int8_t timerID = TIMER_AddSoftTimer(1000, softTimerCallback);
  TIMER_StartSoftTimer(timerID); // start the timer

  int8_t usbTimerID = TIMER_AddSoftTimer(20, usbSoftTimerCallback);
  TIMER_StartSoftTimer(usbTimerID);

  LED_Init(LED0); // Add an LED
  LED_Init(LED1); // Add an LED
  LED_Init(LED2); // Add an LED
  LED_Init(LED3); // Add an LED
  LED_Init(LED5); // Add nonexising LED for test
  LED_ChangeState(LED5, LED_ON);

  KEYS_Init(); // Initialize matrix keyboard

  uint8_t buf[255]; // buffer for receiving commands from PC
  uint8_t len;      // length of command

  // test another way of measuring time delays
  uint32_t softTimer = TIMER_GetTime(); // get start time for delay

  // Initialize USB device stack
  USBD_Init(&USB_OTG_dev,
            USB_OTG_FS_CORE_ID,
            &USR_desc, // USB descriptors
            &USBD_HID_cb, // class callbacks
            &USR_cb); // user callbacks

  while (1) {

    // test delay method
    if (TIMER_DelayTimer(1000, softTimer)) {
      LED_Toggle(LED3);
      softTimer = TIMER_GetTime(); // get start time for delay
    }

    // check for new frames from PC
    if (!COMM_GetFrame(buf, &len)) {
      println("Got frame of length %d: %s", (int)len, (char*)buf);

      // control LED0 from terminal
      if (!strcmp((char*)buf, ":LED0 ON")) {
        LED_ChangeState(LED0, LED_ON);
      }
      if (!strcmp((char*)buf, ":LED0 OFF")) {
        LED_ChangeState(LED0, LED_OFF);
      }
    }

    TIMER_SoftTimersUpdate(); // run timers
    key = KEYS_Update(); // run keyboard

  }
}

#define HID_STEP 10 ///< Cursor step for every move

/**
 * @brief Function return the current move step of HID device
 * @param buf Buffor to fill data.
 */
void getHIDPosition(uint8_t* buf) {

  int8_t x = 0, y = 0;
  uint8_t buttons = 0;
  static uint32_t buttonTimer;
  static uint8_t debounce;

  switch(key) {

  case KEY1:
    x = -HID_STEP;
    y = -HID_STEP;
    break;

  case KEY2:
    x = 0;
    y = -HID_STEP;
    break;

  case KEY3:
    x = HID_STEP;
    y = -HID_STEP;
    break;

  case KEY4:
    x = -HID_STEP;
    y = 0;
    break;

  case KEY6:
    x = HID_STEP;
    y = 0;
    break;

  case KEY7:
    x = -HID_STEP;
    y = HID_STEP;
    break;

  case KEY8:
    x = 0;
    y = HID_STEP;
    break;

  case KEY9:
    x = HID_STEP;
    y = HID_STEP;
    break;

  case KEY_ASTERISK:
//    if (!debounce) {
//      buttonTimer = TIMER_GetTime();
//      debounce = 1;
      buttons |= 0x04; // left mouse button
//    } else if (TIMER_DelayTimer(200, buttonTimer)) {
//      debounce = 0;
//    }
      x = HID_STEP;
      y = HID_STEP;
    break;

  case KEY_HASH:
    buttons |= 0x01; // right mouse button
    x = HID_STEP;
    y = HID_STEP;
    break;

  default:
    x = 0;
    y = 0;

  }

  buf[0] = buttons;
  buf[1] = x;
  buf[2] = y;
  buf[3] = 0;

}

/**
 * @brief Callback for periodic handling of
 * USB stuff.
 */
void usbSoftTimerCallback(void) {

  uint8_t buf[4];

  getHIDPosition(buf);

  if((buf[1] != 0) ||(buf[2] != 0)) {
    USBD_HID_SendReport (&USB_OTG_dev, buf, 4);
  }
}

/**
 * @brief Callback function called on every soft timer overflow
 */
void softTimerCallback(void) {

  static uint8_t counter;

  switch (counter % 3) {

  case 0:
    LED_ChangeState(LED1, LED_OFF);
    LED_ChangeState(LED2, LED_OFF);
    break;

  case 1:
    LED_ChangeState(LED1, LED_ON);
    LED_ChangeState(LED2, LED_OFF);
    break;

  case 2:
    LED_ChangeState(LED1, LED_OFF);
    LED_ChangeState(LED2, LED_ON);
    break;

  }

  println("Test string sent from STM32F4!!!"); // Print test string
  counter++;
}
