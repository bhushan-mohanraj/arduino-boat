#include <SPI.h>
#include <RH_NRF24.h>

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *motor_1 = AFMS.getMotor(3);
Adafruit_DCMotor *motor_2 = AFMS.getMotor(4);

RH_NRF24 nrf24;

int x_mapped_1 = 512;
int x_mapped_2 = 512;

int x_speed_1 = 0;
int x_speed_2 = 0;

bool forward_1 = true;
bool forward_2 = true;

void set_motors()
{
    motor_1->setSpeed(x_speed_1);
    motor_2->setSpeed(x_speed_2);

    if (forward_1) motor_1->run(FORWARD);
    else motor_1->run(BACKWARD);

    if (forward_2) motor_2->run(FORWARD);
    else motor_2->run(BACKWARD);
}

void setup()
{
    Serial.begin(9600);

    while (!Serial);

    if (!nrf24.init()) Serial.println("`init` failed");
    if (!nrf24.setChannel(144)) Serial.println("`setChannel` failed");
    if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
        Serial.println("`setRF` failed");

    AFMS.begin();

    set_motors();
}

void loop()
{
    Serial.println("Sending to nrf24_server");

    uint8_t data[] = "Hello World!";
    nrf24.send(data, sizeof(data));

    nrf24.waitPacketSent();

    uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (nrf24.waitAvailableTimeout(500))
    {
        if (nrf24.recv(buf, &len))
        {
            Serial.print("Got a reply. ");
            Serial.println((char*) buf);

            x_mapped_1 = (int) buf[0];
            x_mapped_2 = (int) buf[1];

            forward_1 = x_1 >= 128;
            forward_2 = x_2 >= 128;

            x_speed_1 = map(abs(x_1 - 128), 0, 128, 0, 255);
            x_speed_2 = map(abs(x_2 - 128), 0, 128, 0, 255);

            set_motors();
        }
        else
        {
            Serial.println("`recv` failed");
        }
    }
    else
    {
        Serial.println("No reply from the server.");
    }

    delay(100);
}
