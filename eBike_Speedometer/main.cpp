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
                             |*| GPIO21          GPIO35 |*|
                             |*| GPIO03          GPIO34 |*|
                             |*| GPIO01          GPIO39 |*| LM35 ADC Channel
                             |*| GPIO22          GPIO36 |*|
                             |*| GPIO23            EN   |*|

     ***************************************************************************************************

     BLUE LED:
      - Blinking - Searching for BLE Connection
      - Solid -  BLE Connected

     RELAYS: Red LED - Power, Green LED - RELAY ON
*/

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define M_PI 3.14159265359  // Redefine PI
#define dt 0.01             // 10 ms Sample rate
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3c // 0x3D for 128x64, 0x3C for 128x32
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define _pulse 13 // Encoder RPM Channel
#define _relay_channel_1 19
#define _stat_led_builtin 2
#define _blu_led_builtin 26
#define _grn_led_builtin 25
#define _red_led_builtin 33
// #define _status_tone_pin 32

#define UPLOAD_DELAY 5000 // how often new readings uploaded to database in milliseconds (60000 = 1 minute)

int count;

void dispSpeedometer();
void dispCruise();
void calcFE();

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

void setup()
{
  Serial.begin(115200);
  // Serial.begin(1);
  pinMode(_relay_channel_1, OUTPUT);
  pinMode(_stat_led_builtin, OUTPUT);
  pinMode(_blu_led_builtin, OUTPUT);
  pinMode(_grn_led_builtin, OUTPUT);
  pinMode(_red_led_builtin, OUTPUT);
  pinMode(_pulse, INPUT);
  display.begin(SCREEN_ADDRESS, true); // Address 0x3C default
  display.setContrast(5);              // Display Contrast
  display.cp437(true);                 // Using IBM standard charset CP437
  display.display();                   // Display = True
  display.clearDisplay();              // Clear the buffer.
  // initBLE();
}

void loop()
{
  // Serial.print(digitalRead(_pulse));
  // Serial.print(" | ");
  // if (digitalRead(_pulse) == LOW)
  // {
  //   count += 1 * (_pulse);
  //   Serial.println(count);
  // }
  // dispSpeedometer();
  // dispCruise();
}

void dispCruise()
{
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.print("Past Trip: 78.9 km");
  display.setCursor(0, 20);
  display.print("Current Trip: 102.8 m");
  display.setCursor(0, 40);
  display.print("RPM: ");
  display.display();
  display.clearDisplay();
}

/*
Use <Preferences.h> lib to save Past trip value to internal memory (max. 8 bytes) add current trip at end
Fuel Pump icon with Spark
Big Battery Icon with map(volt) for fill while charging
*/

/*
void dispSpeedometer()
{
  int s_adc = analogRead(_rpm);
  Serial.print("ADC: ");
  Serial.println(s_adc);
  int veh_spd = map(s_adc, 0, 1023, 0, 25);
  int fill = map(s_adc, 0, 1023, 0, 126);
  display.drawRect(2, 56, 126, 10, SH110X_WHITE);  // (x0, y0, Width, Height, Color)
  display.fillRect(2, 56, fill, 10, SH110X_WHITE); // (x0, y0, Width, Height, Color)
  // Displays Speed limit lines
  display.drawLine(2, 52, 2, 65, SH110X_WHITE); // (x0, y0, x1, y1, Color)
  display.drawLine(25, 52, 25, 65, SH110X_WHITE);
  display.drawLine(50, 52, 50, 65, SH110X_WHITE);
  display.drawLine(75, 52, 75, 65, SH110X_WHITE);
  display.drawLine(100, 52, 100, 65, SH110X_WHITE);
  display.drawLine(127, 52, 127, 65, SH110X_WHITE);
  // display.drawLine(0, 32, 128, 32, SH110X_WHITE);
  // display.drawLine(64, 0, 64, 64, SH110X_WHITE);
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(4);
  display.setCursor(35, 14);
  display.print(veh_spd);
  display.setTextSize(1);
  display.setCursor(82, 34);
  display.print("Km/H");
  // display.drawBitmap(-4, 0, spkr_12pix_bmp, 12, 16, SH110X_WHITE);
  display.display();
  display.clearDisplay();
  // display.clearWriteError();
}
*/
/*!
    @brief
Rect:
(x0, y0)+-------+
        |       |(h)
        +-------+
           (w)
Line:
(x0, y0)-----------(x1, y1)

*/