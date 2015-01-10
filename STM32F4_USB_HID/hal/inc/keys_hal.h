/**
 * @file    keys_hal.h
 * @brief   Matrix keyboard low level functions
 * @date    6 paź 2014
 * @author  Michal Ksiezopolski
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

#ifndef KEYS_HAL_H_
#define KEYS_HAL_H_

#include <inttypes.h>

/**
 * @defgroup  KEYS_HAL KEYS_HAL
 * @brief     Matrix keyboard low level functions
 */

/**
 * @addtogroup KEYS_HAL
 * @{
 */

int8_t KEYS_HAL_ReadRow(void);
void KEYS_HAL_SelectColumn(uint8_t col);
void KEYS_HAL_Init(void);

/**
 * @}
 */

#endif /* KEYS_HAL_H_ */
