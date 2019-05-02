#include <Servo.h>
#include <IRremote.h>

Servo left_motor;
Servo right_motor;
const int left_motor_pin = 52;
const int right_motor_pin = 53;
int fanPin = 13;
int sensorPin = 12;
int RECV_PIN = 2;
IRrecv irrecv(RECV_PIN);
decode_results results;



// Motor range = 52 - 92 - 122
const int top_lmt = 53;
const int zero = 92;
const int bot_lmt = 121;
int throttle = zero;
int incremental = 0;
const int turn_dur = 1000;  // Milliseconds.

bool s = false;
float percent;
float percentage;

/*
 * ff9867 - Center (Stop)
 * ff906F - Plus
 * ffA857 - Minus
 * ffC23D - Right
 * ff02FD - Left
 */

String halt = "ff9867";
String increase = "ff906f";
String decrease = "ffa857";
String right = "ffc23d";
String left = "ff02fd";

void setup(){
  Serial.begin(9600);
  irrecv.enableIRIn();
  left_motor.attach(left_motor_pin);
  right_motor.attach(right_motor_pin);
  pinMode(fanPin, OUTPUT);
  analogWrite(fanPin, 0);
}

void loop(){
  if (Serial.available() > 0){
    int data = Serial.parseInt();
    Serial.println(data);
    left_motor.write(data);
  }
  
  if (irrecv.decode(&results)){
     String code = String(results.value, HEX);
     if (code == halt){
        //New halting code.
       if (throttle > zero){
         incremental = ((throttle - zero)/3);
         for (int i = 0; i<=3 ; i++){
           throttle -= incremental;
           delay(500);
         } else if (throttle < zero){
         for (int i = 0 ; i<= 3 ; i++){
           throttle += incremental;
           delay(500);
         }
     }
       
     } else if (code == increase){
        if (!(throttle < top_lmt)){
          throttle -= 5;
        }
     } else if (code == decrease){
        if (!(throttle > bot_lmt)){
          throttle += 5;
        }
     } 

     Serial.println(throttle);
     left_motor.write(throttle);
     right_motor.write(throttle);
     
     int speed = (throttle - 92);  
     
     if (code == right){
        if (throttle < zero){ 
          if(speed(2.67)<top_lmt){
            left_motor.write(speed*2.67);
          }
        } else{
          right_motor.write(top_lmt-2.67)*speed);
          left_motor.write(top_limit);
        }
     } else if (code == left){
        if (throttle < zero){ 
          if(speed(2.67)<top_lmt){
            right_motor.write(speed*2.67);
          }
        } else{
          left_motor.write(top_lmt-2.67)*speed);
          right_motor.write(top_limit);
        }
     }
     irrecv.resume();
  }
    
    // FAN AND TEMPERATURE SENSOR CODE BELOW
    
     //getting the voltage reading from the temperature sensor
    int reading = analogRead(sensorPin);  
 
    // converting that reading to voltage, for 3.3v arduino use 3.3
    float voltage = reading * 3.3;
    voltage /= 1024.0; 
 
    // print out the voltage
    Serial.print(voltage); Serial.println(" volts");
 
    // now print out the temperature
    float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((voltage - 500mV) times 100)
    Serial.print(temperatureC); Serial.println(" degrees C");
 
    // now convert to Fahrenheit
    float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
    Serial.print(temperatureF); Serial.println(" degrees F");
 
    delay(1000);                                     //waiting a second
    
    // if statement regarding fan control
    if (temperatureF > 85.00) {
      analogWrite(fanPin, 255);
    }
    else {
      analogWrite(fanPin, 1);
    }
}
