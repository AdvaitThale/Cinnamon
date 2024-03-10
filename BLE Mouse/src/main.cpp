#include <Arduino.h>
#include <BleMouse.h>
#define _left_button 4
#define _right_button 16
#define _middle_button 17
#define _fwd_button 18
#define _bck_button 19
#define _up_scroll 34
#define _down_scroll 35
// 16 17 4 18 19 21
#define _conn_ind 33
BleMouse bleMouse;

// const char *deviceName = "Cinnamon BLE Mouse";
// bleMouse.setDeviceName("deviceName");

void setup()
{
  Serial.begin(115200);
  bleMouse.begin();
  pinMode(_left_button, INPUT_PULLUP);
  pinMode(_right_button, INPUT_PULLUP);
  pinMode(_middle_button, INPUT_PULLUP);
  pinMode(_fwd_button, INPUT_PULLUP);
  pinMode(_bck_button, INPUT_PULLUP);
  pinMode(_up_scroll, INPUT);
  pinMode(_down_scroll, INPUT);
  pinMode(_conn_ind, OUTPUT);
  Serial.println("BLE Mouse");
}

void loop()
{
  // Read button states
  bool buttonState1 = digitalRead(_left_button);
  bool buttonState2 = digitalRead(_right_button);
  bool buttonState3 = digitalRead(_middle_button);
  bool buttonState4 = digitalRead(_fwd_button);
  bool buttonState5 = digitalRead(_bck_button);

  // Read scroll wheel state
  int scrollState = 0;
  if (digitalRead(_up_scroll) == HIGH && digitalRead(_down_scroll) == LOW)
  {
    scrollState = -1; // scroll up
  }
  else if (digitalRead(_up_scroll) == LOW && digitalRead(_down_scroll) == HIGH)
  {
    scrollState = 1; // scroll down
  }

  // Emulate mouse actions
  bleMouse.move(0, 0, scrollState); // scroll
  if (buttonState1 == LOW)
  {
    bleMouse.press(MOUSE_LEFT);
  }
  else
  {
    bleMouse.release(MOUSE_LEFT);
  }
  if (buttonState2 == LOW)
  {
    bleMouse.press(MOUSE_RIGHT);
  }
  else
  {
    bleMouse.release(MOUSE_RIGHT);
  }
  if (buttonState3 == LOW)
  {
    bleMouse.press(MOUSE_MIDDLE);
  }
  else
  {
    bleMouse.release(MOUSE_MIDDLE);
  }
  if (buttonState4 == LOW)
  {
    bleMouse.press(MOUSE_FORWARD);
  }
  else
  {
    bleMouse.release(MOUSE_FORWARD);
  }
  if (buttonState5 == LOW)
  {
    bleMouse.press(MOUSE_BACK);
  }
  else
  {
    bleMouse.release(MOUSE_BACK);
  }
  delay(20); // debounce delay
}

// void loop()
// {
//   if (bleMouse.isConnected())
//   {
//     int buttonState = 0; // Initialize the variable to store button state
//     buttonState = digitalRead(_left_button);
//     if (buttonState == HIGH)
//     {
//       Serial.println("Left click");
//       bleMouse.click(MOUSE_LEFT);
//       delayMicroseconds(100);
//     }

//     buttonState = digitalRead(_right_button);
//     if (buttonState == HIGH)
//     {
//       Serial.println("Right click");
//       bleMouse.click(MOUSE_RIGHT);
//       delayMicroseconds(100);
//     }

//     buttonState = digitalRead(_middle_button);
//     if (buttonState == HIGH)
//     {
//       Serial.println("Middle click");
//       bleMouse.click(MOUSE_MIDDLE);
//       delayMicroseconds(100);
//     }

//     buttonState = digitalRead(_fwd_button);
//     if (buttonState == HIGH)
//     {
//       Serial.println("Forward click");
//       bleMouse.click(MOUSE_FORWARD);
//       delayMicroseconds(100);
//     }

//     buttonState = digitalRead(_bck_button);
//     if (buttonState == HIGH)
//     {
//       Serial.println("Backward click");
//       bleMouse.click(MOUSE_BACK);
//       delayMicroseconds(100);
//     }
//     // else
//     // {
//     //   int x = analogRead(x_pos);
//     //   int y = analogRead(y_pos);
//     //   bleMouse.move(x, y, 0);
//     // }
//   }
// }

// Serial.print(digitalRead(_left_button));
// Serial.print(" | ");
// Serial.println(digitalRead(_right_button));
