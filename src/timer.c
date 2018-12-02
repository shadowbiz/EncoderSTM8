#include "stm8s.h"
#include "stm8s_tim4.h"
#include "timer.h"


#define TIM4_PERIOD       124
#define TIM2_PERIOD       250

unsigned int msec = 0,msec_2 = 0;
extern unsigned int faza,current_zad,current,volt_zad;


unsigned char flags = 0,count_adc = 0, x = 0;

extern unsigned char count_enter,c;
 void TIM4_Config(void)
{
   /* Time base configuration */
  TIM4_TimeBaseInit(TIM4_PRESCALER_128, TIM4_PERIOD);  //???? ??? 8 ???,?????????? 1 ??, ??? ARR == 125
  /* Clear TIM4 update flag */
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  /* Enable update interrupt */
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  /* Enable TIM4 */
  TIM4_Cmd(ENABLE);
}

 void TIM2_Config(void)
{
  
   /* Time base configuration */
  TIM2_TimeBaseInit(TIM2_PRESCALER_128, TIM2_PERIOD);         //?????? ??? ??????? ????
  /* Clear TIM4 update flag */
  TIM2_ClearFlag(TIM2_FLAG_UPDATE);
  /* Enable update interrupt */
  TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
  /* Enable TIM2 */
  TIM2_Cmd(DISABLE);
}

 void TIM1_Config(void)
 {
  TIM1_TimeBaseInit(3200,TIM1_COUNTERMODE_UP,1,0);  //??????????  ??, 40 ???
   /* Clear TIM1 update flag */
  TIM1_ClearFlag(TIM1_FLAG_UPDATE);
   /* Enable update interrupt */
  TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
  TIM1_Cmd(DISABLE);
 // TIM1_Cmd(ENABLE);
 }
   
#pragma vector = TIM4_OVR_UIF_vector
__interrupt void Timer4_OVR(void)
{

  if(++msec == 100 )
  {  //GPIO_WriteReverse(GPIOC, (GPIO_Pin_TypeDef)GPIO_PIN_3);
     msec = 0;
     ES_PlaceEvent(EVENT_0_5s_TIMER);
     SetBit(flags,0);
    }
  
 if(++msec_2 == 250 )
 {
  msec_2 = 0;
 if(BitIsSet(flags,ERROR_CURRENT))
  GPIO_WriteReverse(GPIOD, (GPIO_Pin_TypeDef)GPIO_PIN_4);
  }
  
 ENC_PollEncoder();
  
       unsigned char stateEnc = ENC_GetStateEncoder();
      if (stateEnc != 0){
        
        switch (count_enter){
        case 1:
       //?????????? ??????????? ???????? ? ???????? ?????????? current_zad
       if (stateEnc == RIGHT_SPIN && c != 100)
          current_zad++;  
       if (stateEnc == LEFT_SPIN && c != 0) 
          current_zad--;  
       break;
        case 2:
       //?????????? ??????????? ???????? ? ???????? ?????????? volt_zad
       if (stateEnc == RIGHT_SPIN && volt_zad != 0)
          volt_zad--;  
       if (stateEnc == LEFT_SPIN && volt_zad != 160) 
          volt_zad++;   
       break;
        case 3:
       //?????????? ??????????? ???????? ? ???????? ?????????? REG_U_I
       if (stateEnc == RIGHT_SPIN)
          ClearBit(flags,REG_U_I);  
       if (stateEnc == LEFT_SPIN) 
          SetBit(flags,REG_U_I);     
       break;
        case 4:
       //?????????? ??????????? ???????? ? ???????? ?????????? STATE_ENABLE
       if (stateEnc == RIGHT_SPIN)
          ClearBit(flags,STATE_ENABLE);  
       if (stateEnc == LEFT_SPIN) 
          SetBit(flags,STATE_ENABLE);     
       break;
        }
      }
      BUT_Debrief();
      TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  }
  #pragma vector = TIM2_OVR_UIF_vector
__interrupt void Timer2_OVR(void)
{
  if(GPIO_ReadInputPin(GPIOD, (GPIO_Pin_TypeDef)GPIO_PIN_3))    //???? ???????? ??? ??????
  {
   GPIO_WriteLow(GPIOD, (GPIO_Pin_TypeDef)GPIO_PIN_3);          //?????        
   TIM2_SetCounter(0);                                          //?????
   TIM2_Cmd(DISABLE);                                           //????????? ??????
  }
  else 
  {
     GPIO_WriteHigh(GPIOD, (GPIO_Pin_TypeDef)GPIO_PIN_3);       //?????,????????? ????, ? ???
     TIM2_SetAutoreload(125);                                   //???????? ??? ????. ????????
     TIM2_SetCounter(0);
    }
     TIM2_ClearFlag(TIM2_FLAG_UPDATE);
  }
 
  INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11)
{
 /*Start Conversion */
 ADC1_StartConversion();
 /* Clear TIM1 update flag */
 TIM1_ClearFlag(TIM1_FLAG_UPDATE);
 }