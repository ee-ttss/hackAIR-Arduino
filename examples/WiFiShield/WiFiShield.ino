/**
 * @file hackAIR WiFi Shield - Example
 * This example reads data from a sensor and sends it to the hackAIR platform
 * using the hackAIR WiFi Shield on a configurable frequency.
 *
 * @author Thanasis Georgiou
 *
 * This example is part of the hackAIR Arduino Library and is available
 * in the Public Domain.
 */

#include <hackair.h>
#include <hackair_wifi.h>

// How often to measure (in minutes)
#define MEASURING_DELAY 60

// Define sensor
hackAIR sensor(SENSOR_DFROBOT);

// Struct for storing data
struct hackAirData data;

void setup() {
  // Initialize the sensor
  sensor.enablePowerControl();
  sensor.turnOn();
  sensor.begin();
  sensor.clearData(data);

  // Boot WiFi module
  wifi_begin();
  // Wait for WiFi connection
  // At this point you should use your mobile phone to setup the WiFi connection
  wifi_waitForReady();
  // Set authentication token
  wifi_setToken("AUTHORIZATION TOKEN");
}

void loop() {
  // At this point we are either starting up for the first time or after a
  // sleep period.
  // Wait for sensor to settle
  delay(1000 * 30);

  // Make 60 measurements and return the mean values
  sensor.readAverageData(data, 60);
  if (data.error == 1) {
    // no valid data to send. Therefore retry
    // immediately without turning off the sensor.
    return;
  }

  // Send data to the hackAIR platform
  wifi_sendData(data);

  // Turn off sensor while we wait the specified time
  sensor.turnOff();
  delay(MEASURING_DELAY * 60 * 1000UL);
  sensor.turnOn();
}
