
#include "stm8s_it.h"
#include "encoder.h"

/**
  * @brief TRAP Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Top Level Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TLI_IRQHandler, 0)

{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Auto Wake Up Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(AWU_IRQHandler, 1)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Clock Controller Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(CLK_IRQHandler, 2)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTA Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTA_IRQHandler, 3)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTB Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTB_IRQHandler, 4)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTC Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTC_IRQHandler, 5)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTD Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
  if (encoder.debounceTimer == 0)
  {
    if (((GPIO_ReadInputData(GPIOD) & GPIO_PIN_4) == 0x00) &
        (encoder.currentState == POWER_ON))
    {
      if ((GPIO_ReadInputData(GPIOD) & GPIO_PIN_5) == 0x00)
      {
        if (encoder.volume > 0)
        {
          encoder.volume--;
        }
      }
      else 
      {
        if (encoder.volume < 255)
        {
          encoder.volume++;
        }
      }
      /* trigger the set saving */
      encoder.flashTimer = STORE_SET;
      encoder.flashReady = FALSE;
    }

    if ((GPIO_ReadInputData(GPIOD) & GPIO_PIN_6) == 0x00)
    {
      if (encoder.currentState == POWER_OFF)
      {
        encoder.currentState = POWER_ON;
        GPIO_WriteHigh(GPIOD, (GPIO_Pin_TypeDef)GPIO_PIN_1);
      } 
      else if (encoder.currentState == POWER_ON)
      {
        if (encoder.relayOn == TRUE)
        {
          encoder.relayOn = FALSE;
          GPIO_WriteLow(GPIOD, (GPIO_Pin_TypeDef)GPIO_PIN_2);
        }
        else 
        {
          encoder.relayOn = TRUE;
          GPIO_WriteHigh(GPIOD, (GPIO_Pin_TypeDef)GPIO_PIN_2);
        }
      }
    }
  }
  if (encoder.debounceTimer == 0) 
  {
    encoder.debounceTimer = DEBOUNCE;
  }
}

/**
  * @brief External Interrupt PORTE Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTE_IRQHandler, 7)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief SPI Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SPI_IRQHandler, 10)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Timer1 Update/Overflow/Trigger/Break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  if (encoder.debounceTimer > 0)
  {
    encoder.debounceTimer--;
  }
  if (encoder.flashTimer > 0)
  {
    encoder.flashTimer--;
  }
  
  if (encoder.currentState == POWER_ON)
  {
    // long press detection
    if ((GPIO_ReadInputData(GPIOD) & GPIO_PIN_6) == 0x00)
    {
      encoder.timerLongPress++;
    }
    else 
    {
      encoder.timerLongPress = 0;
    }

    if (encoder.timerLongPress > LONG_PRESS)
    {
      encoder.currentState = POWER_OFF;
      GPIO_WriteLow(GPIOD, (GPIO_Pin_TypeDef)GPIO_PIN_1);
      GPIO_WriteLow(GPIOD, (GPIO_Pin_TypeDef)GPIO_PIN_2);
      encoder.timerLongPress = 0;
    }  
  }
    
  TIM1_ClearITPendingBit(TIM1_IT_UPDATE); 
}

/**
  * @brief Timer1 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_CAP_COM_IRQHandler, 12)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Timer2 Update/Overflow/Break Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
   TIM2_ClearITPendingBit  ( TIM2_IT_UPDATE )  ;
 }

/**
  * @brief Timer2 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM2_CAP_COM_IRQHandler, 14)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }



/**
  * @brief UART1 TX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART1_TX_IRQHandler, 17)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }

/**
  * @brief UART1 RX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }

/**
  * @brief I2C Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(I2C_IRQHandler, 19)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Eeprom EEC Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EEPROM_EEC_IRQHandler, 24)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
