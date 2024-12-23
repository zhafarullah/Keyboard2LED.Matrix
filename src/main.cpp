#include <Arduino.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <LedControl.h>
#include "EspUsbHost.h"
#include <Preferences.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4   
#define DATA_PIN 11 
#define CS_PIN 10
#define CLK_PIN 9
#define BUTTON_PIN 4

Preferences preferences;
int mode = 0;
volatile bool buttonPressed = false;

MD_Parola display = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
LedControl lc = LedControl(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

String inputBuffer = "";
String fullTextBuffer = "";
const int maxDisplayChars = 32 / 6;

class MyEspUsbHost : public EspUsbHost {
  void onKeyboardKey(uint8_t ascii, uint8_t keycode, uint8_t modifier) {
    if (mode == 0) {
      if (' ' <= ascii && ascii <= '~') {
        fullTextBuffer += (char)ascii;

        if (fullTextBuffer.length() > maxDisplayChars) {
          inputBuffer = fullTextBuffer.substring(fullTextBuffer.length() - maxDisplayChars);
        } else {
          inputBuffer = fullTextBuffer;
        }

        display.displayClear();
        display.print(inputBuffer.c_str());
        Serial.printf("%c", ascii);
      } else if (ascii == '\r') {
        Serial.println();
      } else if (ascii == 8) {
        if (fullTextBuffer.length() > 0) {
          fullTextBuffer.remove(fullTextBuffer.length() - 1);
        }

        if (fullTextBuffer.length() > maxDisplayChars) {
          inputBuffer = fullTextBuffer.substring(fullTextBuffer.length() - maxDisplayChars);
        } else {
          inputBuffer = fullTextBuffer;
        }

        display.displayClear();
        display.print(inputBuffer.c_str());
        Serial.print("\b \b");
      }
    }
  }
};

MyEspUsbHost usbHost;

void IRAM_ATTR handleButtonPress() {
  buttonPressed = true;
}

void programASetup() {
  Serial.println("Setup Program A");

  display.begin();
  display.setIntensity(10);
  display.displayClear();
  display.setTextAlignment(PA_LEFT);

  for (int device = 0; device < MAX_DEVICES; device++) {
    lc.shutdown(device, false);
    lc.setIntensity(device, 8);
    lc.clearDisplay(device);
  }

  usbHost.begin();
  usbHost.setHIDLocal(HID_LOCAL_Japan_Katakana);
}

void programALoop() {
  usbHost.task();
}

void programBSetup() {
  Serial.println("Setup Program B");

  display.begin();
  display.setIntensity(10);
  display.displayClear();

  for (int device = 0; device < MAX_DEVICES; device++) {
    lc.shutdown(device, false);
    lc.setIntensity(device, 8);
    lc.clearDisplay(device);
  }

  for (int row = 0; row < 8; row++) {
    for (int device = 0; device < MAX_DEVICES; device++) {
      lc.setRow(device, row, B11111111);
    }
    delay(200);
  }

  delay(1000);
}

void programBLoop() {
  if (buttonPressed) {
    return;
  }

  Serial.println("program jalan");

  display.displayText("ZHAFARULLAH AHMAD", PA_CENTER, 50, 1000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  while (!display.displayAnimate()) {
    if (buttonPressed) return;
  }
  display.displayReset();

  display.displayText("5024221004", PA_CENTER, 50, 1000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  while (!display.displayAnimate()) {
    if (buttonPressed) return;
  }
  display.displayReset();

  delay(1000);
}

void switchMode() {
  mode = (mode == 0) ? 1 : 0;
  preferences.putInt("mode", mode);
  Serial.printf("Switching to Program %d and restarting...\n", mode + 1);
  
  ESP.restart();
}

void setup() {
  Serial.begin(115200);
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleButtonPress, FALLING);

  preferences.begin("settings", false);
  mode = preferences.getInt("mode", 0);

  Serial.printf("Current mode: Program %d\n", mode + 1);

  if (mode == 0) {
    programASetup();
  } else {
    programBSetup();
  }
}

void loop() {
  if (buttonPressed) {
    buttonPressed = false;
    switchMode();
  }

  if (mode == 0) {
    programALoop();
  } else if (mode == 1) {
    programBLoop();
  }
}
