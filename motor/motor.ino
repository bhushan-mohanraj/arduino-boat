#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *motor_1 = AFMS.getMotor(2);
Adafruit_DCMotor *motor_2 = AFMS.getMotor(3);

void setup() {
    AFMS.begin();

    motor_1->setSpeed(255);
    motor_1->run(FORWARD);

    motor_2->setSpeed(255);
    motor_2->run(BACKWARD);
}

void loop() {
}
