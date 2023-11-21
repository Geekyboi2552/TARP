//#include <ESP8266WiFi.h>

//#define vibLow 60
//#define vibHigh 100
unsigned long startTime = 0;
unsigned int eventCount = 0;
int heartrate=0;
int heartrate_f=0;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(11,INPUT); //LO+ ke liye
pinMode(10,INPUT); //LO- ke liye
}
//int oldTime=0,newTime=0;
//int calculateBPM()
//{
//  bool flag=true;
//  if(analogRead(A0)>900&&flag=true){
//    newTime=millis();
//    flag=false;
//  }
//  int diff=newTime-oldTime;
//  int currentBPM = 60000 / diff;
//  if(analogRead(A0)>900&&flag=false){
//    oldTime=newTime;
//    flag=true;
//  }
//  return currentBPM;
//}
void loop() {
  // put your main code here, to run repeatedly:
  if((digitalRead(11)==HIGH)||(digitalRead(10)==HIGH)){
 //   Serial.print("");
  }
else{
  int heartbeatValue = analogRead(A0);
Serial.print(heartbeatValue);
Serial.print(" ");
if (heartbeatValue > 750) {
    if (eventCount == 0) {
      startTime = millis();
    } else {
      unsigned long endTime = millis();
      unsigned long timeDifference = endTime - startTime;
 //     Serial.print("Time difference between events: ");
      //Serial.print(timeDifference);
   //   Serial.print(" milliseconds");
      startTime = endTime;
      heartrate=60000/timeDifference;
      if(heartrate<200)
      {
        heartrate_f=heartrate;
      }
    }
  

    eventCount++;
  }
Serial.println(heartrate_f);
 // delay(1000);  // Adjust the delay based on how often you are checking the heartbeat value
}

delay(1);
}
