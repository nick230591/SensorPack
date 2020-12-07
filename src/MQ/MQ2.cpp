#include "MQ2.h"
#include <Arduino.h>

// ==================== PROTECTED-CONSTRUCTOR ====================

MQ2::MQ2(byte apin, byte dpin, GTimer *_timer)
{
    analog_pin = apin;
    digital_pin = dpin;
    timer = _timer;

    value = 0;
    start_calibrate = false;
    start_read = false;
    R0 = 0;
}

// ===================== PUBLIC =======================

float MQ2::read_data()
{
    float permill = 0;    // значение промилле
    if (!start_calibrate) // если калибровки НЕТ, то можем начать измерение
    {
        if (start_read) // если измерение начато
        {
            if (timer->isReady()) // если пришло время измерять
            {
                if (R0) // если калибровка выполнена
                {
                    permill = convert_to_data(read_avg_value()); // получаем значение промилле
                    MQ_PRINTLN_STR_FLOAT("MQ read_permill: ", permill, 3);
                }
                else // если калибровка НЕ выполнена
                    MQ_PRINTLN_STR("You need calibrate MQ-3 sensor!");
                timer->setTimeout(MQ_WAIT_READ_MS); // перезапускаем таймер
            }
            // else
            //     MQ_PRINTLN_STR("MQ timer NOT ready");
        }
        else // если измерение НЕ начато
        {
            start_read = true;                   // запускаем измерение
            timer->setTimeout(MQ_WAIT_READ_MS); // запускаем таймер
            // MQ_PRINTLN_STR("MQ start read");
        }
    }
    // else
    //     MQ_PRINTLN_STR("MQ can't read - it calibrate now");
    return permill;
}

void MQ2::calibrate()
{
    if (start_calibrate) // если калибровка уже начата
    {
        if (timer->isReady()) // если пришло время калиброваться (прогрелась спираль)
        {
            MQ_PRINTLN_STR("");
            float val_f = read_avg_value(); // читываем усредненное напряжение на Rl
            MQ_PRINTLN_STR_FLOAT("MQ calibrate: val_f = ", val_f, 3);
            float Us = val_f / 1024 * MQ_VCC; // напряжение на детекторе
            MQ_PRINTLN_STR_FLOAT("MQ calibrate: Us= ", Us, 3);
            float Rs = (MQ_VCC - Us) / Us; // значение сопротивления сенсора (относительно Rl)
            MQ_PRINTLN_STR_FLOAT("MQ calibrate: Rs = ", Rs, 3);
            R0 = Rs / DIVIDER; // калибруем R0
            MQ_PRINTLN_STR_FLOAT("MQ calibrate: R0 = ", R0, 3);
            start_calibrate = false; // заканчиваем калибровку
            MQ_PRINTLN_STR("MQ calibrate: Cal stop");
        }
        else // если ожидаем прогрева спирали, то в режиме debug - выводим отладочную информацию
        {
#if MQ_DEBUG
            if (millis() % 1000 == 0)
                Serial.print('#');
#endif
        }
    }
    else // если калибровка НЕ начата, то
    {
        start_calibrate = true;              // начинаем калибровку
        timer->setTimeout(MQ_WAIT_HEAT_MS); // запускаем таймер для ожидания прогрева спирали
        MQ_PRINT_STR("MQ calibrate: Cal start: ");
    }
}

//========================== GET ================================
bool MQ2::get_start_calibrate() const
{
    return start_calibrate;
}

bool MQ2::get_start_read() const
{
    return start_read;
}

float MQ2::get_R0() const {
    return R0;
}

//========================== SET ================================

void MQ2::set_start_calibrate(bool state)
{
    start_calibrate = state;
}

void MQ2::set_start_read(bool state)
{
    start_read = state;
}

//========================== PROTECTED ================================

float MQ2::read_Rs(float _value) const{
    float Us = _value / 1024 * MQ_VCC; // напряжение на нагрузочом резисторе
    // MQ_PRINTLN_STR_FLOAT("MQ convert_to_promill: Us = ", Us, 3);
    float Rs = (MQ_VCC - Us) / Us; // значение сопротивления сенсора (относительно Rl)
    // MQ_PRINTLN_STR_FLOAT("MQ convert_to_promill: Rs = ", Rs, 3);
    return Rs;
}

//========================== PRIVATE ================================

float MQ2::read_avg_value() const
{
    long val = 0;
    for (byte i = 0; i < MQ_AVG; i++)
    {
        val += analogRead(analog_pin);
        // MQ_PRINTLN_STR_INT("MQ: value = ", value);
    }
    return (float)val / MQ_AVG;
}

float MQ2::convert_to_data(float _value) const
{
    return 0.9 * pow(read_Rs(_value) / get_R0(), -1.48); // пересчитываем через выведенную в Excel формулу
}