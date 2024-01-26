#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define _temp_sensitivity 16384.0 // +-2g = 16384 LSB/g
#define _meth_sensitivity 131.0   // 250 degrees/s = 131 LSB/degrees/s
#define dt 0.01                   // 10 ms Sample rate
#define SCREEN_WIDTH 128          // OLED display width, in pixels
#define SCREEN_HEIGHT 64          // OLED display height, in pixels
#define OLED_RESET -1             // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3c       // 0x3D for 128x64, 0x3C for 128x32
#define BUZZER 11                 // BUZZER Pin
#define _toggle_reset 3           // Toggle to display angular values
#define CENTRE_LED 12             // LED Pin
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define BITMAP_BALL_HEIGHT 5
#define BITMAP_BALL_WIDTH 5
static const unsigned char PROGMEM ball_4pix_bmp[] =
    {
        B01110,
        B10001,
        B10001,
        B10001,
        B01110,
};

int16_t AcX, Tmp; // 16-bit ints
float previousTime, currentTime, elapsedTime;
float AcErrorX, AcErrorY, AcErrorZ; // Calibration variables  // AcErrorX = -1, AcErrorY = 0. , AcErrorZ = -1/360

void display_statusBar();
void printever();
void getMeth();
void initialize();

void setup()
{
  Serial.begin(115200);
  pinMode(BUZZER, OUTPUT);              // Set to OUTPUT for Buzzer Pin
  pinMode(_toggle_reset, INPUT_PULLUP); // Toggle to display angular values
  pinMode(CENTRE_LED, OUTPUT);          // Set to OUTPUT for LED Pin

  display.begin(SCREEN_ADDRESS, true); // Address 0x3C default
  display.setContrast(5);              // Display Contrast
  display.cp437(true);                 // Using IBM standard charset CP437
  display.display();                   // Display = True
  display.clearDisplay();              // Clear the buffer.
  // display.setCursor(64, 32);
  // display.print("AUGHH");
  // initialize();
}

void loop()
{
  if (digitalRead(_toggle_reset) == HIGH) // Angles displayed after keypressed
  {
    printever();
  }
  else
  {
    display_statusBar();
  }
  // printAngles();
  // getAngle();
}

void display_statusBar()
{
  // display.drawLine(0, 32, 128, 32, SH110X_WHITE);
  // display.drawLine(64, 0, 64, 64, SH110X_WHITE);
  // if (pitch == 32 && roll == 64)
  // {
  //   display.setTextSize(1);
  //   display.setTextColor(SH110X_WHITE);
  //   display.setCursor(62, 29);                    // Adjusted Centre
  //   display.print("+");                           // Absolute Centre
  //   display.drawCircle(64, 32, 12, SH110X_WHITE); // Centre Reference Circle
  //   digitalWrite(CENTRE_LED, HIGH);               // LED ON
  //   digitalWrite(LED_BUILTIN, HIGH);              // LED ON
  // }
  // else
  // {
  //   display.fillCircle(roll, pitch, 12, SH110X_WHITE); // ((270) -90 <= Roll >= 90, 90 <= Pitch >= -90 (270))
  //   digitalWrite(CENTRE_LED, LOW);                     // LED OFF
  //   digitalWrite(LED_BUILTIN, LOW);                    // LED OFF
  // }
  // display.drawLine(0, 32, 128, 32, SH110X_WHITE);
  // display.drawLine(64, 0, 64, 64, SH110X_WHITE); // x, y, x1, y1
  // display.drawBitmap(roll, pitch, ball_12pix_bmp, BITMAP_BALL_HEIGHT, BITMAP_BALL_WIDTH, SH110X_WHITE);
  display.display();
  display.clearDisplay();
  // display.clearWriteError();
}

void printever()
{
  // getMeth();
  display.drawLine(0, 54, 128, 54, SH110X_WHITE);
  display.drawLine(64, 15, 64, 48, SH110X_WHITE); // Vertical Line
  display.setTextSize(3);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(7, 25);
  display.print("24");
  // display.drawBitmap(43, 23, ball_4pix_bmp, BITMAP_BALL_HEIGHT, BITMAP_BALL_WIDTH, SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(43, 23);
  display.print("o");
  display.display();
  display.clearDisplay();
  // display.drawLine(0, 0, 128, 64, SH110X_WHITE);
  // display.clearWriteError();
}

// void getMeth()
// {
//   Wire.beginTransmission(MPU);
//   Wire.write(0x3B); // Accelerometer Measurement Register
//   Wire.endTransmission(false);
//   Wire.requestFrom(MPU, 6, true); // 14

//   AcX = Wire.read() << 8 | Wire.read(); // 8 bit shift
//   AcY = Wire.read() << 8 | Wire.read();
//   AcZ = Wire.read() << 8 | Wire.read();

//   xAng = map(AcX, -ACCELEROMETER_SENSITIVITY, ACCELEROMETER_SENSITIVITY, -90, 90);
//   yAng = map(AcY, -ACCELEROMETER_SENSITIVITY, ACCELEROMETER_SENSITIVITY, -90, 90);
//   zAng = map(AcZ, -ACCELEROMETER_SENSITIVITY, ACCELEROMETER_SENSITIVITY, -90, 90);

//   int xAngle = map(AcX, minVal, maxVal, -90, 90);
//   int yAngle = map(AcY, minVal, maxVal, -90, 90);
//   int zAngle = map(AcZ, minVal, maxVal, -90, 90);

//   r = RAD_TO_DEG * (atan2(-yAngle, -zAngle) + PI); // Angular Conversion rad to deg
//   p = RAD_TO_DEG * (atan2(-xAngle, -zAngle) + PI);
//   y = RAD_TO_DEG * (atan2(-yAngle, -xAngle) + PI);

//   Serial.print("Roll= ");
//   Serial.print(r);
//   Serial.print(" | Pitch= ");
//   Serial.print(p);
//   Serial.print(" | Yaw= ");
//   Serial.println(y);
// }

void initialize()
{
  Serial.println("Gathering IMU Data...");
  digitalWrite(BUZZER, HIGH);
  delay(85);
  digitalWrite(BUZZER, LOW);
  tone(BUZZER, 2093);
  delay(200);
  noTone(BUZZER);
  digitalWrite(BUZZER, HIGH);
  delay(100);
  digitalWrite(BUZZER, LOW);
}