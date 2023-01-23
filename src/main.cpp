#include <Arduino.h>
#include <DHT.h>
#include <time.h>
#include <sensor_constants.h>
#include <serial_helper.h>
#include <data_helper.h>
#include <wifi_helper.h>
#include <sensor_ntp.h>
#include <sensor_udp.h>
#include <sensor_webserver.h>

#define PIN_DHT22 2
#define DHT_TYPE DHT22

bool read_sensor_values(void);

DHT dht(PIN_DHT22, DHT_TYPE);

time_t last_measurement = 0;
bool ntp_setup = false;

void setup()
{
    Serial.begin(115200);
    dht.begin();

    wait_for_serial_connect();

    init_data();

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

    if (WiFi.isConnected())
    {
        init_ntp();
        ntp_setup = true;
        delay(5000);

        init_webserver();
    }
}

void loop()
{
    if (is_wifi_setup())
    {
        if (!WiFi.isConnected())
        {
            wifi_connect();
        }

        if (!ntp_setup && WiFi.isConnected())
        {
            init_ntp();
            delay(5000);

            init_webserver();
        }
    }

    time(&last_measurement);
    bool day_update = read_sensor_values();

    print_data_to_serial(!day_update);

    time_t now;
    do
    {   
        handle_client();
        time(&now);
        delay(100);
    } while (now < (last_measurement + SLEEP_TIME));
}

bool read_sensor_values(void)
{
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    udp_send_data(humidity, temperature);

    return add_values(humidity, temperature);
}
