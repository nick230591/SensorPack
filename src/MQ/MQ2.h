/*
   MQ - базовый класс для работы с детектороми серии MQ
   VCC -  VCC
   GND -   GND
   D0 -     цифровой пин - выход компаратора - HIGH при достижении на сенсоре определенной границы задаваемой потенциометром
   A0 -     аналоговый пин - напряжение на нагрузочном сопротивлении
   Скотников Никита, АО НИИПП, 4 отдел

   Версия 1.0 (30.11.20) - написан основной код программы
*/

#pragma once

#include <Arduino.h>
#include "GyverTimer.h"
#include "config_MQ.h"

class MQ2
{

   uint16_t value;      // текущее значение сенсора
   uint8_t analog_pin;  // аналоговый пин
   uint8_t digital_pin; // цифровой пин
   float R0;            // сопротивление сенсора при НУ
   // flags
   bool start_calibrate; // начата калибровка
   bool start_read;      // начато чтение данных
   // timers
   GTimer *timer; // указатель на таймер таймер

public:
   MQ2(byte apin, byte dpin, GTimer *_timer); // конструктор класса
   float read_data();                        // считать значение промилле
   void calibrate();                         // калибровка - измерение сопротивления R0
   // getters
   bool get_start_calibrate() const;
   bool get_start_read() const;
   float get_R0() const;
   // setters
   void set_start_calibrate(bool state);
   void set_start_read(bool state);

private:
   // other_func
   float read_avg_value() const;              // считать усредненное значение с аналогового входа
   float read_Rs(float _value) const;         // считаль значение сопротивления сенсора
   float convert_to_data(float _value) const; // конвертировать _value в data
};