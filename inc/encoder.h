#ifndef	__ENCODER_H__
#define	__ENCODER_H__

#define LONG_PRESS      5
#define DEBOUNCE        1
#define STORE_SET       10

#define DIG_PORT        GPIOD
#define DIG1_PIN        GPIO_PIN_3
#define DIG2_PIN        GPIO_PIN_2
#define DIG3_PIN        GPIO_PIN_1

enum state { POWER_OFF, POWER_ON };

typedef struct
{
  u16           debounceTimer;
  u8            flashTimer;
  bool          flashReady;
  u8            volume;
  u8            currentState;
  bool          relayOn;
  u8            timerLongPress;
} encoder_;

extern encoder_ encoder;

void seven_segment(void);

#endif  