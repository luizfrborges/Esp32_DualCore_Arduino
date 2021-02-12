#include "headers/tasklibs.h"


void blink02(){
  //Serial.print("Task1 running on core ");
  //Serial.println(xPortGetCoreID());
    pinMode(2, OUTPUT);

  for(;;){
    digitalWrite(2, HIGH);
    delay(1000);
    digitalWrite(2, LOW);
    delay(1000);
  } 
}

//Task2code: blinks an LED every 700 ms
void blink04(){
  //Serial.print("Task2 running on core ");
  //Serial.println(xPortGetCoreID());
    pinMode(4, OUTPUT);
  for(;;){
    digitalWrite(4, HIGH);
    delay(700);
    digitalWrite(4, LOW);
    delay(700);
  }
}