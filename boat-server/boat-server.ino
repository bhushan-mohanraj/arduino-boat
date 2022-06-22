#include <SPI.h>
#include <RH_NRF24.h>

RH_NRF24 nrf24;

// The values from the joysticks along the horizontal axis.
int x_1 = 512;
int x_2 = 512;

int x_mapped_1 = 128;
int x_mapped_2 = 128;

void setup()
{
    Serial.begin(9600);

    while (!Serial);

    if (!nrf24.init()) Serial.println("`init` failed");
    if (!nrf24.setChannel(144)) Serial.println("`setChannel` failed");
    if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
        Serial.println("`setRF` failed");

    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
}

void loop()
{
    if (nrf24.available())
    {
        uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);

        if (nrf24.recv(buf, &len))
        {
            Serial.print("Got a request. ");
            Serial.println((char*) buf);

            x_1 = analogRead(A0);
            x_2 = analogRead(A1);

            // Map the `x` values to numbers within the range
            // that can be sent as eight-bit integers (ASCII characters).
            x_mapped_1 = map(x_1, 0, 1023, 0, 255);
            x_mapped_2 = map(x_2, 0, 1023, 0, 255);

            uint8_t data[] = {x_mapped_1, x_mapped_2};

            nrf24.send(data, sizeof(data));
            nrf24.waitPacketSent();

            Serial.print("Sent a reply.");
        }
        else
        {
            Serial.println("`recv` failed");
        }
    }
}
