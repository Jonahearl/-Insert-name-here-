//This is the first iteration of firmware for Galvanised Kaas PID sourced from https://randomdamon.blogspot.com/2015/12/diy-hot-end-arduino-pid-control.html
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

const float Temp = 3950; // should match the Beta Coefficient of the thermistor

// digital 4 a switch (first temp with full PID line one)
// digital 3 a switch (the spinning begins line two)
// mosfet is on pin 9 for pwm
// steppa is on 5 for step and 6 for dir
// digital 7 for heat led


void setup() {
  // put your setup code here, to run once:
lcd.init();
lcd.setBacklight();
Serial.begin(9600);
pinMode(5,OUTPUT); // Step
pinMode(6,OUTPUT); //Dir
pinMode(4,INPUT_PULLUP); // temp
pinMode(3,INPUT_PULLUP); // spin
}

void loop() {
  // put your main code here, to run repeatedly:

int analogValue = analogRead(A0);
float celsius = 1 / (log(1 / (1023. / analogValue - 1)) / Temp + 1.0 / 298.15) - 273.15;
Serial.print("Temperature: ");
Serial.print(celsius);
Serial.println(" ℃");
delay(1000);

digitalWrite(6,HIGH); // Set Dir high

if (digitalRead(4) == 1){
  lcd.setCursor(0, 0);
  lcd.print(celcius);
  Serial.println("trying temp");
  //analogWrite(5, 128);
  if (celcius) =< 195 {
  analogWrite(9, 128);
  delay(2300);
  else if (celcius) => 200 {
    analogWrite(9, 0);
    delay(2100);

  }


  }
  
}
if (digitalRead(3) == 1) {
   digitalWrite(5,HIGH); // Output high
    delay(10); // Wait
    digitalWrite(5,LOW); // Output low
    delay(100); // Wait

}


}
