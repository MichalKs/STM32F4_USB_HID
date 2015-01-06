/**
  ******************************************************************************
  * @file    stm32fxxx_it.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    19-March-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides all exceptions handler and peripherals interrupt
  *          service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usb_core.h"
#include "usbd_core.h"
#include "usbd_hid_core.h"
#include "usb_conf.h"

__IO uint32_t remote_wakeup =0;
/* Private function prototypes -----------------------------------------------*/
extern USB_OTG_CORE_HANDLE           USB_OTG_dev;
extern uint32_t USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);


/**
* @brief  This function handles EXTI15_10_IRQ Handler.
* @param  None
* @retval None
*/
void EXTI0_IRQHandler(void) {

  if (EXTI_GetITStatus(EXTI_Line0) != RESET) {

//    if (USB_OTG_dev.dev.DevRemoteWakeup) {

      if((USB_OTG_dev.cfg.low_power)&&(USB_OTG_dev.dev.device_status==USB_OTG_SUSPENDED)) {

        /* Reset SLEEPDEEP and SLEEPONEXIT bits */
        SCB->SCR &= (uint32_t)~((uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));

        /* After wake-up from sleep mode, reconfigure the system clock */
        /* After wake-up from STOP reconfigure the system clock */

        /* Enable HSE */
        RCC_HSEConfig(RCC_HSE_ON);

        /* Wait till HSE is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);

        /* Enable PLL */
        RCC_PLLCmd(ENABLE);

        /* Wait till PLL is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

        /* Select PLL as system clock source */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

        /* Wait till PLL is used as system clock source */
        while (RCC_GetSYSCLKSource() != 0x08);

        USB_OTG_UngateClock(&USB_OTG_dev);
      }
      printf("******************Remote wakeup\r\n");
      USB_OTG_ActiveRemoteWakeup(&USB_OTG_dev);
//      USB_OTG_dev.dev.device_status = USB_OTG_dev.dev.device_old_status;
//      remote_wakeup = 1;
//    }
    /* Clear the EXTI line pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);

    printf("******************EXTI0\r\n");
  }
}

/**
* @brief  This function handles EXTI15_10_IRQ Handler.
* @param  None
* @retval None
*/
void OTG_FS_WKUP_IRQHandler(void) {

  if(USB_OTG_dev.cfg.low_power) {
    /* Reset SLEEPDEEP and SLEEPONEXIT bits */
    SCB->SCR &= (uint32_t)~((uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));

    /* After wake-up from sleep mode, reconfigure the system clock */
    RCC_HSEConfig(RCC_HSE_ON);
    
    /* Wait till HSE is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
    
    /* Enable PLL */
    RCC_PLLCmd(ENABLE);
    
    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
    
    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    
    /* Wait till PLL is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x08);

    USB_OTG_UngateClock(&USB_OTG_dev);
  }
  EXTI_ClearITPendingBit(EXTI_Line18);
  printf("******************In wakeup\r\n");
}

/**
* @brief  This function handles OTG_HS Handler.
* @param  None
* @retval None
*/
void OTG_FS_IRQHandler(void) {
  USBD_OTG_ISR_Handler (&USB_OTG_dev);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
