#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *motor_1 = AFMS.getMotor(2);
Adafruit_DCMotor *motor_2 = AFMS.getMotor(3);

int x = 0;
int y = 0;

int x_speed = 255;
int y_speed = 255;

void setup() {
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);

    AFMS.begin();

    motor_1->setSpeed(255);
    motor_1->run(FORWARD);

    motor_2->setSpeed(255);
    motor_2->run(BACKWARD);

    Serial.begin(9600);
}

void loop() {
    x = analogRead(A0);
    y = analogRead(A1);

    x_speed = map(x, 0, 1023, 0, 255);
    y_speed = map(y, 0, 1023, 0, 255);

    motor_1->setSpeed(x);
    motor_2->setSpeed(y);
}
