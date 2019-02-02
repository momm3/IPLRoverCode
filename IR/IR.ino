#include <IRremote.h>

int RECV_PIN = 2;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop()
{
  if (irrecv.decode(&results)){ 
    String value = String(results.value, HEX);
    Serial.println(value);
    irrecv.resume(); // Receive the next value
    }
}

/*
 * FF9867 - Center (Stop)
 * FF906F - Plus
 * FFA857 - Minus
 * FFC23D - Right
 * FF02FD - Left
 */
