#include <Arduino.h>
#include <Preferences.h>
#define butt 34
Preferences preferences;

void setup()
{
  Serial.begin(115200);
  pinMode(butt, INPUT);
  preferences.begin("my-app", false); // Open Preferences with namespace "my-app", Read/write mode enabled
  float val = 13.456;                   // Value to store
  preferences.putFloat("my_val", float(val));
  Serial.println("Stored ");

  float d_val = preferences.getFloat("my_val", 0.0); // Get the value, if the key does not exist, return default
  Serial.print("Retrieved float: ");
  Serial.println(d_val);
  preferences.end(); // Close Preferences
}

void loop() {
  if(digitalRead(butt) == 1){
    preferences.begin("my-app", false); // Open Preferences with namespace "my-app", Read/write mode enabled
    float r_val = preferences.getFloat("my_val");
    preferences.putFloat("my_val", r_val + 1);
    preferences.end(); // Close Preferences
  }
}
