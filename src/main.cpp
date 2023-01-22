#include <Arduino.h>
#include <DHT.h>
#include <sensor_constants.h>
#include <serial_helper.h>
#include <data_helper.h>
#include <wifi_helper.h>
#include <sensor_ntp.h>

#define PIN_DHT22 2
#define DHT_TYPE DHT22

DHT dht(PIN_DHT22, DHT_TYPE);

void read_sensor_values(void);

void setup()
{
    Serial.begin(115200);
    dht.begin();

    wait_for_serial_connect();

    if (!is_setup())
    {
        serial_setup();
    }
    else
    {
        reset_sensor();
    }

    if (!is_wifi_setup())
    {
        serial_wifi_setup();
    }

    if (is_wifi_setup())
    {
        wifi_connect();
    }

    if(WiFi.isConnected()) {
        init_ntp();
    }
}

void loop()
{
    if (!is_setup())
    {
        serial_setup();
    }

    if (!is_wifi_setup())
    {
        serial_wifi_setup();
    }

    if (is_wifi_setup())
    {
        wifi_connect();
    }

    if(WiFi.isConnected()) {
        init_ntp();
    }

    read_sensor_values();

    print_data_to_serial();

    // TODO: broadcast data to UDP

    // TODO: webserver

    // TODO: handle with timer interrupt for more precise distances
    delay(SLEEP_TIME);
}

void read_sensor_values(void)
{
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    add_values(humidity, temperature);
}
