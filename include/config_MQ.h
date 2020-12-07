/*
   Конфигурационный файл для детектора алкоголя MQ

   Версия 1.0 (30.11.20) - написан основной код программы

   Скотников Никита, АО НИИПП, 4 отдел
*/
#pragma once

// #include <Arduino.h>

// НЕ НУЖНО РАСКОММЕНТИРОВАТЬ ДАННУЮ СТРОКУ
// ВСЕ НАСТРОКИ ЗАДАЮТСЯ В ФАЙЛЕ .INI
// #define MQ_DEBUG 0 

// defin-ы для debug - вывода
// TODO добавить NAME для каждого датчика MQ
#if MQ_DEBUG
#define MQ_PRINT_STR(x) Serial.print(F(x))
#define MQ_PRINTLN_NAME_STR(name, str) Serial.print(F(name)); Serial.println(str);
#define MQ_PRINTLN_STR(x) Serial.println(F(x))
#define MQ_PRINT_FLOAT(x, n) Serial.print(x, n)
#define MQ_PRINTLN_FLOAT(x, n) Serial.println(x, n)
#define MQ_PRINT_INT(x) Serial.print(x)
#define MQ_PRINTLN_INT(x) Serial.println(x)
#define MQ_PRINTLN_STR_FLOAT(str, num, n) Serial.print(F(str)); Serial.println(num, n)
#define MQ_PRINTLN_STR_INT(str, num) Serial.print(F(str)); Serial.println(num)
#else
#define MQ_PRINT_STR(x)
#define MQ_PRINTLN_STR(x)
#define MQ_PRINT_FLOAT(x, n)
#define MQ_PRINTLN_FLOAT(x, n)
#define MQ_PRINT_INT(x)
#define MQ_PRINTLN_INT(x)
#define MQ_PRINTLN_STR_FLOAT(str, num, n)
#define MQ_PRINTLN_STR_INT(str, num)
#endif

#define MQ_VCC 5.0        // значение напряжения к которому подключены датчики газа серии MQ
#define MQ_AVG 100        // усреднение для считывания показаний MQ-3
#define MQ_WAIT_HEAT_MS 10000 // ожидание прогрева спирали датчика, мс
#define MQ_WAIT_READ_MS 1000  // периодичность считывания данных с датчика, мс
#define DIVIDER 5          // калибровочный коэффициент для MQ-3
