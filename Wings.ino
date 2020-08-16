/*
 Name:		Wings.ino
 Created:	01.08.2020 23:30:15
 Author:	Eva
*/

#include <ServoSmooth.h>

//пины для линейного актуатора
#define pinEna 9
#define pinIn1 8
#define pinIn2 7

//пины для сервоприводов
#define pinServoL 6
#define pinServoR 5

#define pinLight 3

ServoSmooth servoL;
ServoSmooth servoR;

unsigned long timing=0;
byte powerLight;

void setup()
{
    SetupServo();
    SetupLinear();
}

bool forward = true;
void loop() 
{
    if (millis() - timing > 5000)
    {
        timing = millis();
        if (forward)
        {
            powerLight = 255;
            MoveLinear(255);
            ServosRotate(0);
           
        }
        else
        {
            powerLight = 100;
            MoveLinear(-255);
            ServosRotate(80);
           // analogWrite(pinLight,150);
        }
        forward = !forward;
    }

    analogWrite(pinLight,powerLight );
    servoL.tick();
    servoR.tick();

}

void SetupLinear()
{
    // Установка всех управляющих пинов в режим выхода
    pinMode(pinEna, OUTPUT);
    pinMode(pinIn1, OUTPUT);
    pinMode(pinIn2, OUTPUT);
    Stop();
}
void SetupServo()
{
    servoL.attach(pinServoL, 600, 2400);  // 600 и 2400 - длины импульсов, при которых серво поворачивается максимально в одну и другую сторону, зависят от самой серво
    servoR.attach(pinServoR, 600, 2400);

    // ограничить скорость
    servoL.setSpeed(10);
    servoR.setSpeed(10);

    // установить ускорение (разгон и торможение)
    servoL.setAccel(0.2);
    servoR.setAccel(0.2);

    // отключить автоотключение (detach) при достижении целевого угла (по умолчанию включено)
    servoL.setAutoDetach(true);
    servoR.setAutoDetach(true);
}

/// <summary>
/// Поворачивает сервы зеркально
/// </summary>
/// <param name="newPos">градусы(0..180)</param>
void ServosRotate(int newPos)
{
    //// берём с потенцометра значение 0-180
    newPos = newPos < 0 ? 0 : (newPos > 180 ? 180 : newPos);
    // и отправляем на серво
    servoL.setTargetDeg(newPos);
    servoR.setTargetDeg(180-newPos);
}

/// <summary>
/// двигает актуатор
/// </summary>
/// <param name="speed">скорость от -255 до 255</param>
void MoveLinear(int speed=255)
{
    if (speed>0)
    {
        SetSpeedLinear(speed);
        // Задаём направление для актуатора
        digitalWrite(pinIn1, HIGH);
        digitalWrite(pinIn2, LOW);
    }
    else
    {
        SetSpeedLinear(speed*-1);
        // Задаём направление для актуатора
        digitalWrite(pinIn1, LOW);
        digitalWrite(pinIn2, HIGH);
    }
}

void SetSpeedLinear(int speed)
{
    if (speed > 255) speed = 255;
    else if (speed < 0) speed = 0;

    analogWrite(pinEna, speed); // Устанавливаем скорость мотора
}
void Stop()
{
    // Команда остановки двум моторам
    digitalWrite(pinIn1, LOW);
    digitalWrite(pinIn2, LOW);
}
void Pause()
{
    // Команда резкой остановки двум моторам
    digitalWrite(pinIn1, HIGH);
    digitalWrite(pinIn2, HIGH);
}
