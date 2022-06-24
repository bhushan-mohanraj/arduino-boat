// Libraries for the transceiver.
#include <SPI.h>
#include <RH_NRF24.h>

// Libraries for the motors.
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"


// Set up the transceiver.
// The channel number for the transceiver.
#define TRANSCEIVER_CHANNEL 144


// Initialize the transceiver.
// Create an object to interact with the transceiver.
RH_NRF24 nrf24;

// Initialize the motors.
// Create objects to interact with the motors.
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *motor_1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor_2 = AFMS.getMotor(2);

int joystick_1_x_mapped = 512;
int joystick_2_x_mapped = 512;

int motor_1_speed = 0;
int motor_2_speed = 0;

bool is_motor_1_forward = true;
bool is_motor_2_forward = true;


// Set the motor speeds and directions according to the variable values.
void set_motors()
{
    motor_1->setSpeed(motor_1_speed);
    motor_2->setSpeed(motor_2_speed);

    if (is_motor_1_forward) motor_1->run(FORWARD);
    else motor_1->run(BACKWARD);

    if (is_motor_2_forward) motor_2->run(FORWARD);
    else motor_2->run(BACKWARD);
}


void setup()
{
    Serial.begin(9600);

    // Wait for the serial monitor to load.
    while (!Serial);

    // Connect to the transceiver.
    if (!nrf24.init())
        Serial.println("`init` failed");
    if (!nrf24.setChannel(TRANSCEIVER_CHANNEL))
        Serial.println("`setChannel` failed");
    if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
        Serial.println("`setRF` failed");

    // Connect to the motors and run them.
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

            joystick_1_x_mapped = (int) buf[0];
            joystick_2_x_mapped = (int) buf[1];

            is_motor_1_forward = joystick_1_x_mapped >= 128;
            is_motor_2_forward = joystick_2_x_mapped >= 128;

            motor_1_speed = map(abs(joystick_1_x_mapped - 128), 0, 128, 0, 255);
            motor_2_speed = map(abs(joystick_2_x_mapped - 128), 0, 128, 0, 255);

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

    delay(400);
}
