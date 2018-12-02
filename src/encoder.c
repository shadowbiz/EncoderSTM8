#include "stm8s.h"
#include "encoder.h"

volatile unsigned char bufEnc = 0; //????? ????????

void ENC_InitEncoder(void)
{
  GPIO_Init(GPIOB, (GPIO_Pin_TypeDef)GPIO_PIN_4, GPIO_MODE_IN_PU_NO_IT);//???? ??? ????????????? ????????
  GPIO_Init(GPIOB, (GPIO_Pin_TypeDef)GPIO_PIN_5, GPIO_MODE_IN_PU_NO_IT);
}

void ENC_PollEncoder(void)
{
  static unsigned char stateEnc; 	//?????? ?????????????????? ????????? ????????
  unsigned char tmp;  
  unsigned char currentState = 0;



  if(GPIO_ReadInputPin(GPIOB, (GPIO_Pin_TypeDef)GPIO_PIN_4)) {SetBit(currentState,0);}
  if(GPIO_ReadInputPin(GPIOB, (GPIO_Pin_TypeDef)GPIO_PIN_5)) {SetBit(currentState,1);}
  //if ((PIN_Enc & (1<<Pin1_Enc))!= 0) {SBR(currentState,0);}
  //if ((PIN_Enc & (1<<Pin2_Enc))!= 0) {SBR(currentState,1);}

  
  tmp = stateEnc;
  if (currentState == (tmp & b00000011)) return;

  stateEnc = (tmp<<2)|currentState;

  if (tmp == b11100001) bufEnc = LEFT_SPIN;
  if (tmp == b11010010) bufEnc = RIGHT_SPIN;
}

unsigned char ENC_GetStateEncoder(void)
{
  unsigned char tmp = bufEnc;
  
  if (tmp)
  {
    bufEnc = 0;
    return tmp;
  }
  return 0;  
}


