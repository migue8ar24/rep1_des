// C++ code
//
#include <Adafruit_LiquidCrystal.h>

int seconds = 0;
int analogPin = 0;
int val = 0;

Adafruit_LiquidCrystal lcd_1(0);

void setup()
{
  Serial.begin(9600);
  
  //lcd_1.begin(16, 2);
}

void loop()
{
  /*
  lcd_1.setCursor(0, 1);
  lcd_1.print(seconds);
  lcd_1.setBacklight(1);
  delay(500); // Wait for 500 millisecond(s)
  lcd_1.setBacklight(0);
  delay(500); // Wait for 500 millisecond(s)
  seconds += 1;
  */
  val = analogRead(analogPin);
  Serial.println(val);
}