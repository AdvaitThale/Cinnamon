/*

    **************************************************************************************************
    *                                   ESP32 PINOUT & SENSORS                                       *
    **************************************************************************************************

                         3V3 |*|                        |*| 5V
                         GND |*|                        |*| GND
                             |*| GPIO15          GPIO13 |*| IR-Encoder
                         BLE |*| GPIO02          GPIO12 |*|
                             |*| GPIO04          GPIO14 |*| PID Fan PWM Channel
                             |*| GPIO16          GPIO27 |*|
                             |*| GPIO17          GPIO26 |*| Status LED
                             |*| GPIO05          GPIO25 |*| Status LED
                             |*| GPIO18          GPIO33 |*| Status LED
                             |*| GPIO19          GPIO32 |*|
                         SDA |*| GPIO21          GPIO35 |*|
                      BLE TX |*| GPIO03          GPIO34 |*|
                      BLE RX |*| GPIO01          GPIO39 |*| LM35 ADC Channel
                             |*| GPIO22          GPIO36 |*|
                         SCL |*| GPIO23            EN   |*|

     ***************************************************************************************************

     BLUE LED:
      - Blinking - Searching for BLE Connection
      - Solid -  BLE Connected

     RELAYS: Red LED - Power, Green LED - RELAY ON
*/

#include <Arduino.h>
#include <Preferences.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <LiquidCrystal_I2C.h>

#define M_PI 3.14159265359  // Redefine PI
#define DT_T 0.01             // 10 ms Sample rate
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3c // 0x3D for 128x64, 0x3C for 128x32

#define IN_PULSE 13 // Encoder RPM Channel
#define RELAY_CHANNEL_1 19
#define STATUS_LED_BUILTIN 2
#define BLUE_LED_BUILTIN 26
#define GREEN_LED_BUILTIN 25
#define RED_LED_BUILTIN 33
// #define _status_tone_pin 32

#define UPLOAD_DELAY 5000 // how often new readings uploaded to database in milliseconds (60000 = 1 minute)

int count;
int past_trip = 231;

// Function Prototypes
void dispSpeedometer(); 
void dispCruise();
void calcFE();

Preferences pref;
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
  Serial.begin(115200);
  // Serial.begin(1);
  pref.begin("str", false);
  pinMode(RELAY_CHANNEL_1, OUTPUT);
  pinMode(STATUS_LED_BUILTIN, OUTPUT);
  pinMode(BLUE_LED_BUILTIN, OUTPUT);
  pinMode(GREEN_LED_BUILTIN, OUTPUT);
  pinMode(RED_LED_BUILTIN, OUTPUT);
  //////////////////////////////////////////
  pref.putInt("Past-Trip", past_trip);
  Serial.println("Integer Stored!");
  int e_val = pref.getUInt("Past-Trip");
  Serial.print("Past Trip: ");
  Serial.println(e_val);
  display.begin(SCREEN_ADDRESS, true); // Address 0x3C default
  display.setContrast(5);              // Display Contrast
  display.cp437(true);                 // Using IBM standard charset CP437
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(1, 1);
  display.println("Past Trip: ");
  display.setCursor(30, 1);
  display.println(e_val);
  display.display();      // Display = True
  display.clearDisplay(); // Clear the buffer.
  // initBLE();
}

/*
 * Retreive Past FE from EEPROM
 * Retreive Ideal FE from EEPROM
 * Update & Store Current into Past FE to EEPROM
 */

// Module to estimate Fuel Efficiency
void calcFE()
{
  CONFIG_ESP_INSIGHTS_CLOUD_POST_MIN_INTERVAL_SEC; // Replace all small variable to ALL CAPITALISED
  FUEL_EFFICIENCY_CALC_INTRVL_SEC;
  
  lcd.setCursor(0, 0); // Range
  lcd.print("RANGE: ");
  lcd.setCursor(7, 0);
  lcd.print(pres_range);
  lcd.setCursor(13, 0);
  lcd.print("KM");
  lcd.setCursor(0, 1); // Fuel Efficiency
  lcd.println("FE: ");
  lcd.setCursor(4, 1);
  lcd.print(pres_fe);
  lcd.setCursor(9, 1);
  lcd.print("KM/kWH");

  /*
  +----------------+
  |RANGE: xxx.x KM |
  |FE: xx.x KM/kWH |
  +----------------+

  */
}

void loop() {}

// Function for Tone Generation
// void tone(int pin, int freq, int duration)
// {
//   ledcSetup(0, freq, 8);
//   ledcAttachPin(pin, 0);
//   ledcWriteTone(0, freq);
//   delay(duration);
//   ledcDetachPin(pin);
// }
// void noTone(int pin)
// {
//   ledcDetachPin(pin);
// }

/*
// Initialize BLE
void initWiFi()
{
  WiFi.begin(WIFI_SSID, BLE_PASSWORD);
  Serial.print("Connecting to Wi-Fi ..");
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(_stat_led_builtin, !digitalRead(_stat_led_builtin));
    Serial.print('.');
    delay(500);
  }
  digitalWrite(_stat_led_builtin, HIGH);
  Serial.print("Connected to ");
  Serial.println(BLE_addr);
  // Serial.println(BLE.mac());
  Serial.println();
  // tone(_status_tone_pin, 2000, 200); // 4093
  // noTone(_status_tone_pin);
  // tone(_status_tone_pin, 4093, 85); // 4093
  // noTone(_status_tone_pin);
  // tone(_status_tone_pin, 2000, 200); // 4093
  // noTone(_status_tone_pin);

  // tone(_status_tone_pin, 1800, 300); // 4093
  // noTone(_status_tone_pin);
  // tone(_status_tone_pin, 7000, 200); // 4093
  // noTone(_status_tone_pin);
}
*/

/*
Use <Preferences.h> lib to save Past trip value to internal memory (max. 8 bytes) add current trip at end
Fuel Pump icon with Spark
Big Battery Icon with map(volt) for fill while charging
*/