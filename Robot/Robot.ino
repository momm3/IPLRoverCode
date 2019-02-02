#include <Servo.h>
#include <IRremote.h>

Servo left_motor;
Servo right_motor;
const int left_motor_pin = 3;
const int right_motor_pin = 10;
int RECV_PIN = 2;
IRrecv irrecv(RECV_PIN);
decode_results results;


// Motor range = 52 - 92 - 122
const int top_lmt = 53;
const int zero = 92;
const int bot_lmt = 121;
int throttle = zero;
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
        // Serial.print("Halting... ");
        throttle = zero;
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
     
     if (code == right){
        if (throttle < zero){ 
          right_motor.write(throttle + 5);
          delay(turn_dur);
          right_motor.write(throttle);
        } else{
          left_motor.write(throttle - 5);
          delay(turn_dur);
          left_motor.write(throttle);
        }
     } else if (code == left){
        if (throttle < zero){ 
          left_motor.write(throttle + 5);
          delay(turn_dur);
          left_motor.write(throttle);
        } else{
          right_motor.write(throttle - 5);
          delay(turn_dur);
          right_motor.write(throttle);
        }
     }

     irrecv.resume();
  }
}


// void left_motor(int throttle_per){ //
//   m1.write(convert_throttle(throttle_per));
// }

//float convert_throttle(int percent){  // -100 to 100
//  float value;
//
//  if (percent > 0){
//    value = zero - (percent / 100) * (dir_2 - zero);
//  }
//  else if(percent < 0){
//    value = zero - (percent / 100) * (zero - dir_1);
//  }
//  else{
//    value = zero;
//  }
//
//  return value;
//}
