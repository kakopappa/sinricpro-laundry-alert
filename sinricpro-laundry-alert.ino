#ifdef ENABLE_DEBUG
       #define DEBUG_ESP_PORT Serial
       #define NODEBUG_WEBSOCKETS
       #define NDEBUG
#endif 

#include <Arduino.h>
#ifdef ESP8266 
       #include <ESP8266WiFi.h>
#endif 
#ifdef ESP32   
       #include <WiFi.h>
#endif

#include "CircularBuffer.h"

#include "SinricPro.h"
#include "WashingMachine.h"

#define WIFI_SSID         "YOUR-WIFI-SSID"    
#define WIFI_PASS         "YOUR-WIFI-PASSWORD"
#define APP_KEY           "YOUR-APP-KEY"      // Should look like "de0bxxxx-1x3x-4x3x-ax2x-5dabxxxxxxxx"
#define APP_SECRET        "YOUR-APP-SECRET"   // Should look like "5f36xxxx-x3x7-4x3x-xexe-e86724a9xxxx-4c4axxxx-3x3x-x5xe-x9x3-333d65xxxxxx"
#define DEVICE_ID         "YOUR-DEVICE-ID"    // Should look like "5dc1564130xxxxxxxxxxxxxx"

#define BAUD_RATE         9600 

#define SAMPLE_SIZE             50  // No of samples to evaluate for the start/stop of a wash/dry circle
#define NO_OF_POSTIVE_SAMPLES   30  // samples within SAMPLE_SIZE that are postive or negative to confirm start/stop the wash/dry circle
#define VIBRATION_SENSOR_PIN    D2  // Vibration sensor (SW420) is connected to D2.

#define IDLE        0
#define SPIN_CYCLE  1

WashingMachine &washingMachine = SinricPro[DEVICE_ID];
CircularBuffer<bool, SAMPLE_SIZE> samples;
bool detected = 0;
byte state = 0;

// setup function for WiFi connection
void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }

  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  
  Serial.printf("connected!\r\n[WiFi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());
}

// setup function for SinricPro
void setupSinricPro() {
  // setup SinricPro
  SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); }); 
  SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}

void setup() {
  Serial.begin(BAUD_RATE);

  setupWiFi();
  setupSinricPro();
  
  Serial.println("Goto: IDLE");
  state = IDLE;
}

// @state true detected, false not-detected
bool vibration(bool state) {
  int positive_count = 0;
  
  for(int i = 0; i < SAMPLE_SIZE ; i++){
      if(samples.pop() == state)
          positive_count++;
  }
   
  if(positive_count >= NO_OF_POSTIVE_SAMPLES) {      
      return true;
  }
  else {
      return false;
  }
}

// PushNotificationController
void sendPushNotification(String notification) {
  washingMachine.sendPushNotification(notification);
}

// ModeController
void updateMode(String instance, String mode) {
  washingMachine.sendModeEvent(instance, mode, "PHYSICAL_INTERACTION");
}
 
void loop() {
  SinricPro.handle();
  
  // Read the measurement value.
  detected = analogRead(VIBRATION_SENSOR_PIN); 
  samples.push(detected);

  //Serial.print("detected:");
  Serial.println(detected);

  switch (state) {
    case IDLE:
      if(vibration(true)) {
        Serial.println("Vibration detected! Go to SPIN_CYCLE");
        updateMode("modeInstance1", "spinning");
        sendPushNotification("Laundry will be ready in 10 mins!");
        state = SPIN_CYCLE;
        return;
      }
      break;
    case SPIN_CYCLE:
      if(vibration(false)) {
        Serial.println("Vibration not detected! Go to IDLE");
        sendPushNotification("Time to pick up the laundry!!");
        updateMode("modeInstance1", "idle");
        state = IDLE;
        return;
      }
      break; 
  }

  delay(100);
} 

//int sensor = A0;
//
//void setup() {
//  Serial.begin(9600);
//}
//
//void loop() {
//  int val = analogRead(sensor);
//  Serial.println(val);
//  delay(50);
//}
