/*
  ========================================
  ESP32 MAGNETIC MOVER LED HARDWARE CONTROLLER & WEB TRACKER
  Blue LED = North Pole (at detected IR sensor)
  Red LED  = South Pole (at next IR sensor)
  ========================================

  FUNCTION:
  100% REAL HARDWARE DRIVEN (Zero Simulation):
  - Directly reads physical IR sensors on pins: 34, 35, 36, 39, 32, 33
  - Real-time hardware control of 6 Blue LEDs and 6 Red LEDs (no PWM glitching)
  - Live Web UI Dashboard served over Wi-Fi (SSID: uyya) and AP (ESP32-Bot-Tracker)
    reflecting real physical sensor and LED states.
*/

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "web_dashboard.h"

// ==========================================
// IR SENSOR GPIO PINS
// ==========================================
const int irPins[6] = {
  34, 35, 36, 39, 32, 33
};

// ==========================================
// BLUE LEDS: NORTH POLE
// ==========================================
const int blueLEDs[6] = {
  23, 22, 21, 19, 18, 25
};

// ==========================================
// RED LEDS: SOUTH POLE
// ==========================================
const int redLEDs[6] = {
  26, 27, 14, 13, 12, 15
};

// Most IR modules output LOW when detecting an object
const int IR_ACTIVE_STATE = LOW;

// Web Server on port 80
WebServer server(80);

// Real-time hardware telemetry state
volatile int currentActiveJunction = 1;
volatile int lastActiveJunction = 0;
volatile int lastDetectedSensor = -1;
unsigned long lastSerialPrintTime = 0;

void allLEDsOff() {
  for (int i = 0; i < 6; i++) {
    digitalWrite(blueLEDs[i], LOW);
    digitalWrite(redLEDs[i], LOW);
  }
}

// HTTP API Handlers for Live Hardware Telemetry
void handleRoot() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send_P(200, "text/html", DASHBOARD_HTML);
}

void handleStatus() {
  int northIdx = (currentActiveJunction > 0) ? (currentActiveJunction - 1) : 0;
  int southIdx = (northIdx < 5) ? (northIdx + 1) : -1;

  char jsonBuffer[256];
  snprintf(jsonBuffer, sizeof(jsonBuffer),
    "{\"active_junction\":%d,\"north\":%d,\"south\":%d,\"detected\":%s}",
    currentActiveJunction,
    (northIdx + 1),
    (southIdx >= 0 ? southIdx + 1 : 0),
    (lastDetectedSensor >= 0 ? "true" : "false")
  );

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", jsonBuffer);
}

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("\n=== ESP32 Magnetic Mover Hardware Starting ===");

  // Initialize GPIO pins
  for (int i = 0; i < 6; i++) {
    pinMode(irPins[i], INPUT);
    pinMode(blueLEDs[i], OUTPUT);
    pinMode(redLEDs[i], OUTPUT);
  }

  // Ensure all LEDs are off initially
  allLEDsOff();

  // Configure Wi-Fi Credentials
  const char* staSSID = "uyya";
  const char* staPass = "Shamball";
  const char* apSSID  = "ESP32-Bot-Tracker";
  const char* apPass  = "Shamball";

  // Dual Station + AP mode
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(staSSID, staPass);
  WiFi.softAP(apSSID, apPass);

  Serial.print("Connecting to Wi-Fi SSID: ");
  Serial.println(staSSID);

  unsigned long startWait = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startWait < 8000) {
    delay(400);
    Serial.print(".");
  }

  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(">>> Wi-Fi Connected Successfully! <<<");
    Serial.print("Local Network Dashboard URL: http://");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println(">>> Wi-Fi connection timed out. Fallback AP active. <<<");
  }

  Serial.print("Direct AP SSID:             ");
  Serial.println(apSSID);
  Serial.print("Direct AP Dashboard URL:    http://");
  Serial.println(WiFi.softAPIP());

  // Configure Web Server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/status", HTTP_GET, handleStatus);
  server.begin();
  Serial.println("HTTP Web Server ready on port 80.");
}

void loop() {
  // Handle Web Dashboard clients
  server.handleClient();

  // Read all physical IR sensors directly from hardware
  int detectedSensor = -1;
  for (int i = 0; i < 6; i++) {
    if (digitalRead(irPins[i]) == IR_ACTIVE_STATE) {
      detectedSensor = i;
      break;
    }
  }

  lastDetectedSensor = detectedSensor;

  // Direct, glitch-free hardware LED control
  for (int i = 0; i < 6; i++) {
    bool blueOn = (i == detectedSensor);
    bool redOn  = (detectedSensor >= 0 && detectedSensor < 5 && i == detectedSensor + 1);

    digitalWrite(blueLEDs[i], blueOn ? HIGH : LOW);
    digitalWrite(redLEDs[i], redOn ? HIGH : LOW);
  }

  // Real position acquisition
  if (detectedSensor >= 0) {
    currentActiveJunction = detectedSensor + 1;

    // Rate-limited clean Serial logging (prevents serial buffer flood)
    if (currentActiveJunction != lastActiveJunction && (millis() - lastSerialPrintTime >= 250)) {
      lastSerialPrintTime = millis();
      lastActiveJunction = currentActiveJunction;

      Serial.print("Hardware IR Sensor Active: Station ");
      Serial.print(currentActiveJunction);
      Serial.print(" | Blue LED: GPIO ");
      Serial.print(blueLEDs[detectedSensor]);
      if (detectedSensor < 5) {
        Serial.print(" | Red LED: GPIO ");
        Serial.println(redLEDs[detectedSensor + 1]);
      } else {
        Serial.println(" | End of Track");
      }
    }
  }
}