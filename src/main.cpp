#include <Arduino.h>
#include "DHT.h"

#define DHT_TYPE DHT22
#define PIN_DHT22 2

DHT dht(PIN_DHT22, DHT_TYPE);

float humidity;
float temperature;

void setup()
{
    Serial.begin(115200);
    dht.begin();
}

void loop()
{
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print("%, Temperature: ");
    Serial.print(temperature);
    Serial.println("°C");

    delay(1000);
}
