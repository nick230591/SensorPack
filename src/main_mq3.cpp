#include <Arduino.h>
#include "GyverTimer.h"
#include "GyverTimers.h"
#include "Debounce.h"

#include "MQ/MQ3.h"

#include "config.h"
#include "config_MQ3.h"

// ******************************* TIMERS *********************************
GTimer SlowEventTimer(MS, timers::update_slow_event_interval); // интервальный таймер для МЕДЛЕННЫХ событий
GTimer FastEventTimer(MS, timers::update_fast_event_interval); // интервальный таймер для БЫСТРЫХ событий
GTimer CalibrateMQTimer(MS);                                   // таймаут таймер для датчика MQ-3

// ******************************* SENSORS *********************************
MQ3 mq3(A0, 3, &CalibrateMQTimer); // датчик MQ-3

// ******************************* BUTTONS *********************************
Debounce cal_button(CALIBRATE_BUTTON, 80, true, true); // калибровка
Debounce meas_button(MEAS_BUTTON, 80, true, true);     // измерение для MQ-3

// ******************************* EVENTS*********************************
bool slow_event_arr[slow_event_count] = {false}; // массив для медленных событий
bool fast_event_arr[fast_event_count] = {false}; // массив для быстрых событий

// ******************************* SETUP *********************************
void setup()
{
#if MQ_DEBUG
  Serial.begin(9600);
  Serial.println(F("Hello! I'm started!"));
#endif
  // настраиваем аппаратный таймер 
  Timer1.setPeriod(timers::update_button_state_interval); // настраиваем Timer1 - каждые 25 мс будет обновляться состояние джойстика
  Timer1.enableISR();                                     // разрешаем прерывания от Timer1
}

// ******************************* LOOP *********************************
void loop()
{

  if (SlowEventTimer.isReady()) // если сработал таймер для МЕДЛЕННЫХ событий
  { 
    // калибровка MQ-3
    if (slow_event_arr[CALIBRATE_MQ3]) {
      mq3.calibrate();
    }
    else if (mq3.get_start_calibrate()) {
      mq3.calibrate();
    }

    // измерение MQ-3
    if (slow_event_arr[MEASHURE_MQ3])
      mq3.read_data();
  }
  if (FastEventTimer.isReady()) // если сработал таймер для БЫСТРЫХ событий
  {
    if (fast_event_arr[UPDATE_BUTTON_STATE])
    {
      // обновляем состояние кнопок
      // одинарное нажатие запускает таймер обратного отсчета, пока таймер не отсчитает
      // любые манипуляции с кнопкой не дадут никакого эффекта
      if (cal_button.read())  // для калибровки MQ-3
        slow_event_arr[CALIBRATE_MQ3] = true;
      else if (!mq3.get_start_calibrate())
        slow_event_arr[CALIBRATE_MQ3] = false;

      // при ПЕРВОМ клике (нажатии и отжатии) на кнопку ЗАПУСКАЕТСЯ цикл измерения
      // каждую секунду будут считаны опказания датчика
      // при ВТОРОМ клике на кнопку измрения ПРОЕКРАЩАЮТСЯ
      if (meas_button.count() == 1) // для измерения MQ-3
      {
        slow_event_arr[MEASHURE_MQ3] = true;
      }
      else if (meas_button.count() == 2)
      {
        slow_event_arr[MEASHURE_MQ3] = false;
        mq3.set_start_read(false);
        meas_button.resetCount();
        // MQ_PRINTLN_STR("MQ stop read");
      }

      fast_event_arr[UPDATE_BUTTON_STATE] = false;
    }
  }
}

// Прерывание А таймера 1 - каждые 10 мс! - обновляем состояние кнопок
ISR(TIMER1_A)
{
  fast_event_arr[UPDATE_BUTTON_STATE] = true;
}