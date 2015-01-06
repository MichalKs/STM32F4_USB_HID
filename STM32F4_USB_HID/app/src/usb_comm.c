/**
 * @file    usb_comm.c
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

#include <comm.h>
#include <fifo.h>
#include <stdio.h>

#ifndef DEBUG
  #define DEBUG
#endif

#ifdef DEBUG
  #define print(str, args...) printf("USB_COMM--> "str"%s",##args,"\r")
  #define println(str, args...) printf("USB_COMM--> "str"%s",##args,"\r\n")
#else
  #define print(str, args...) (void)0
  #define println(str, args...) (void)0
#endif

/**
 * @defgroup  USB_COMM USB_COMM
 * @brief     Communication with PC functions.
 */

/**
 * @addtogroup USB_COMM
 * @{
 */

#define USB_COMM_BUF_LEN     2048    ///< USB_COMM buffer lengths
#define USB_COMM_TERMINATOR '\r'     ///< USB_COMM frame terminator character

static uint8_t rxBuffer[USB_COMM_BUF_LEN]; ///< Buffer for received data.
static uint8_t txBuffer[USB_COMM_BUF_LEN]; ///< Buffer for transmitted data.

static FIFO_TypeDef rxFifo; ///< RX FIFO
static FIFO_TypeDef txFifo; ///< TX FIFO

static uint8_t gotFrame;  ///< Nonzero signals a new frame (number of received frames)

uint8_t USB_COMM_TxCallback(uint8_t* c);
void    USB_COMM_RxCallback(uint8_t c);

/**
 * @brief Initialize communication terminal interface.
 *
 * @param baud Required baud rate
 */
void USB_COMM_Init(uint32_t baud) {

  // Initialize RX FIFO
  rxFifo.buf = rxBuffer;
  rxFifo.len = USB_COMM_BUF_LEN;
  FIFO_Add(&rxFifo);

  // Initialize TX FIFO
  txFifo.buf = txBuffer;
  txFifo.len = USB_COMM_BUF_LEN;
  FIFO_Add(&txFifo);

}

/**
 * @brief Send a char to USART2.
 * @details This function can be called in stubs.c _write
 * function in order for printf to work
 *
 * @param c Char to send.
 */
void USB_COMM_Putc(uint8_t c) {
  // disable IRQ so it doesn't screw up FIFO count - leads to errors in transmission

//  FIFO_Push(&txFifo,c); // Put data in TX buffer

  // enable IRQ again
}
void USB_COMM_Puts(char* str) {
  VCP_DataTx(str, strlen(str));
}
/**
 * @brief Get a char from USART2
 * @return Received char.
 * @warning Blocking function! Waits until char is received.
 */
uint8_t USB_COMM_Getc(void) {

  uint8_t c;

  while (FIFO_IsEmpty(&rxFifo) == 1); // wait until buffer is not empty
  // buffer not empty => char was received

//  USART_ITConfig(USART2, USART_IT_RXNE, DISABLE); // disable RX interrupt

  FIFO_Pop(&rxFifo,&c); // Get data from RX buffer

//  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // enable RX interrupt

  return c;
}
/**
 * @brief Get a complete frame from USART2 (nonblocking)
 * @param buf Buffer for data (data will be null terminated for easier string manipulation)
 * @param len Length not including terminator character
 * @retval 0 Received frame
 * @retval 1 No frame in buffer
 * @retval 2 Frame error
 * TODO Add maximum length checking so as not to overflow
 */
uint8_t USB_COMM_GetFrame(uint8_t* buf, uint8_t* len) {

  uint8_t c;
  *len = 0; // zero out length variable

  if (gotFrame) {
    while (1) {

      // no more data and terminator wasn't reached => error
      if (FIFO_IsEmpty(&rxFifo)) {
        *len = 0;
        println("Invalid frame");
        return 2;
      }
      FIFO_Pop(&rxFifo, &c);
      buf[(*len)++] = c;

      // if end of frame
      if (c == USB_COMM_TERMINATOR) {
        (*len)--; // length without terminator character
        buf[*len] = 0; // USART terminator character converted to NULL terminator
        break;
      }

    }
    gotFrame--;
    return 0;

  } else {

    return 1;
  }

}
/**
 * @brief Callback for receiving data from PC.
 * @param c Data sent from lower layer software.
 */
void USB_COMM_RxCallback(uint8_t c) {

  uint8_t res = FIFO_Push(&rxFifo, c); // Put data in RX buffer

  // Checking res to ensure no buffer overflow occurred
  if ((c == USB_COMM_TERMINATOR) && (res == 0)) {
    gotFrame++;
  }
}
/**
 * @brief Callback for transmitting data to lower layer
 * @param c Transmitted data
 * @retval 0 There is no more data in buffer (stop transmitting)
 * @retval 1 Valid data in c
 */
uint8_t USB_COMM_TxCallback(uint8_t* c) {

  if (FIFO_Pop(&txFifo, c) == 0) { // If buffer not empty
    return 1;
  } else {
    return 0;
  }

}

/**
 * @}
 */
