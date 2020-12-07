/*
   Конфигурационный файл для ВСЕЙ программы

   Версия 1.0 (30.11.20) - написан основной код программы

   Скотников Никита, АО НИИПП, 4 отдел
*/
#pragma once

// ***************************** TIMERS *******************************
namespace timers
{
  const uint32_t update_button_state_interval = 10000; // период обновления состояния кнопок, МКС - для Timer2
  const uint16_t update_slow_event_interval = 100;     // период для МЕДЛННЫХ событий, мс - крутится в  loop
  const uint8_t update_fast_event_interval = 25;       // период быстрых событий, мс - крутится в  loop
};                                                     // namespace timers

// ***************************** BUTTONS *******************************
enum buttons
{
  CALIBRATE_BUTTON = 7, // для калибровки
  MEAS_BUTTON = 6,      // для измерения
};

// ***************************** EVENTS *******************************
// медленные события - выполняются с периодом update_slow_event_interval
enum slow_event_name
{
  CALIBRATE_MQ3, // откалибровать MQ3
  MEASHURE_MQ3,  // измерить показания MQ3
};
const byte slow_event_count = 2; // ОБНОВЛЯТЬ! при добавлении новых событий в slow_event_name

// быстрые события - выполняются с периодом update_fast_event_interval
enum fast_event_name
{
  UPDATE_BUTTON_STATE,  // обновить состояния кнопок
};
const byte fast_event_count = 1; // ОБНОВЛЯТЬ! при добавлении новых событий в fast_event_name