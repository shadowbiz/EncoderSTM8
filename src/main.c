

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "encoder.h"

static void CLK_Config(void);
static void GPIO_Config(void);
static void TIM1_Config(void);
static void TIM2_Config(void);
static void delay(__IO uint32_t nCount);

void store_set(void);

encoder_ encoder;

void main(void)
{
  uint8_t f_data;
  
  encoder.debounceTimer = DEBOUNCE;
  encoder.volume = 0;
  encoder.currentState = POWER_OFF;
  encoder.flashReady = TRUE;
  encoder.relayOn = FALSE;
  
  CLK_Config();
  GPIO_Config();
  TIM1_Config();
  TIM2_Config();
  
  GPIO_WriteLow(GPIOD, (GPIO_Pin_TypeDef)GPIO_PIN_1);
  GPIO_WriteLow(GPIOD, (GPIO_Pin_TypeDef)GPIO_PIN_2);
  //GPIO_WriteHigh(GPIOD, (GPIO_Pin_TypeDef)GPIO_PIN_3);
  
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  f_data = FLASH_ReadByte(0x00004000);
  //f_data = (f_data << 8) + FLASH_ReadByte(0x00004001);
  //f_data = (f_data << 8) + FLASH_ReadByte(0x00004002);
  //f_data = (f_data << 8) + FLASH_ReadByte(0x00004003);
  FLASH_Lock(FLASH_MEMTYPE_DATA);
  
  encoder.volume = f_data;
  
  enableInterrupts(); 
  
  /* Infinite loop */
  while (1)
  {
    if ((encoder.flashReady == FALSE) && (encoder.debounceTimer == 0))
    {
        encoder.flashReady = TRUE;
        store_set();
    }
  }
  
}

void store_set()
{
  FLASH_Unlock(FLASH_MEMTYPE_DATA)  ;
  FLASH_ProgramWord(0x00004000, (uint8_t)encoder.volume);
  FLASH_Lock(FLASH_MEMTYPE_DATA)  ;
}

static void CLK_Config(void)
{
  CLK_DeInit();
  
  /* Clock divider to HSI/1 */
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV8);
  CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
  
  /* Output Fcpu on CLK_CCO pin */
//  CLK_CCOConfig(CLK_OUTPUT_MASTER);
}

/**
  * @brief  Configure GPIO for buttons available on the evaluation board
  * @param  None
  * @retval None
  */
static void GPIO_Config(void)
{
  // encoder
  GPIO_Init(GPIOD, (GPIO_Pin_TypeDef)(GPIO_PIN_4 | GPIO_PIN_6) , GPIO_MODE_IN_PU_IT);
  //GPIO_Init(GPIOD, (GPIO_Pin_TypeDef)(GPIO_PIN_5) , GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIOD, (GPIO_Pin_TypeDef)(GPIO_PIN_5) , GPIO_MODE_IN_PU_IT);
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_FALL_ONLY);

  
  //GPIO_Init(DIG_PORT, (GPIO_Pin_TypeDef)(DIG1_PIN | DIG2_PIN | DIG3_PIN) , GPIO_MODE_OUT_PP_LOW_FAST);
  //GPIO_Init(DIG_PORT, (GPIO_Pin_TypeDef)(DIG1_PIN | DIG2_PIN | DIG3_PIN) , GPIO_MODE_OUT_PP_HIGH_FAST);
  
}

/**
  * @brief  Configure TIM1 to generate interrupt
  * @param  None
  * @retval None
  */
static void TIM1_Config(void)
{
    CLK_PeripheralClockConfig (CLK_PERIPHERAL_TIMER1 , ENABLE);     
    TIM1_DeInit(); 
    TIM1_TimeBaseInit(800, TIM1_COUNTERMODE_DOWN, 1000-1, 0); //timer freq = (clock CPU/800) -> 1bit = 1uS -> 92*1uS=92uS 
    TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE); 
    TIM1_Cmd(ENABLE); 
}

/**
  * @brief  Configure TIM1 to generate interrupt
  * @param  None
  * @retval None
  */
static void TIM2_Config(void)
{
    CLK_PeripheralClockConfig (CLK_PERIPHERAL_TIMER2 , ENABLE);     
    TIM2_DeInit(); 
    TIM2_TimeBaseInit(TIM2_PRESCALER_2, 20000); //timer freq = (clock CPU/8) -> 1bit = 1uS -> 100*1uS=100uS 
    TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE); 
    TIM2_Cmd(ENABLE); 
}


static void delay(__IO uint32_t nCount)
 {
   /* Decrement nCount value */
   while (nCount != 0)
   {
     nCount--;
   }
 }

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
