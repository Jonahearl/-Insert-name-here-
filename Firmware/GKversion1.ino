//This is the first iteration of firmware for Galvanised Kaas PID sourced from https://randomdamon.blogspot.com/2015/12/diy-hot-end-arduino-pid-control.html

#include <PID_v1.h>
// Analog output pin
#define outputPin 9
// thermistor analog pin
#define TSPIN A0
// how many samples to take and average
#define SAMCOUNT 5
// how long between pid/sampling
#define SAMPLETIME 1000
//Define Variables we'll be connecting to
double Setpoint, currentTemp, Output;
//Specify the links and initial tuning parameters
PID myPID(&currentTemp, &Output, &Setpoint,15,.3,0, DIRECT);

const int stepPin = 3; 
const int dirPin = 4; 
void setup() {
  Serial.begin(9600);
  analogReference(EXTERNAL);
  pinMode(outputPin, OUTPUT);
  //initialize PID setpoint *C
  Setpoint = 235;
  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(SAMPLETIME);
  //pid Autotuner
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
}
void loop() {
if (digitalRead(8) == 1) {
  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < 200; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(500); 
    delay(1000);
  
}
    if (Serial.available() > 0) {
      // get incoming byte:
      Setpoint = Serial.parseFloat();
    }
    uint8_t i;
    double average = 0;
    // take N samples in a row, with a slight delay
    for (i = 0; i < SAMCOUNT; i++) {
      average += analogRead(TSPIN);
      delay(10);
    }
    average /= SAMCOUNT;
    currentTemp=resistanceToC(inputToResistance(average));
    myPID.Compute();
    analogWrite(outputPin, Output);

    Serial.print("Set Point: ");
    Serial.print(Setpoint);
    Serial.println(" *C)");
    Serial.print("Temperature: ");
    Serial.print(currentTemp);
    Serial.println(" *C)");
    Serial.print("PID output ");
    Serial.println(Output);
    delay(SAMPLETIME);
  }
  double inputToResistance(double input) {
    // funtion to convert the input value to resistance
    // the value of the 'other' resistor
    double SERIESRESISTOR = 10000;
    input = 1023 / input - 1;
    return SERIESRESISTOR / input;
  }
  double resistanceToC(double resistance) {
    // funtion to convert resistance to c
    // temp/resistance for nominal
    double THERMISTORNOMINAL = 118000;
    double TEMPERATURENOMINAL = 25;
    // beta coefficent
    double BCOEFFICIENT = 3950;
    double steinhart;
    steinhart = resistance / THERMISTORNOMINAL;     // (R/Ro)
    steinhart = log(steinhart);                  // ln(R/Ro)
    steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
    steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
    steinhart = 1.0 / steinhart;                 // Invert
    steinhart -= 273.15;   // convert to C
    return steinhart;
  }
}
