#include <ESP8266WiFi.h>
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
SoftwareSerial NODEMCU(0,1);
LiquidCrystal_I2C lcd(0x27,16,2); // LCD pins: RS, E, D4, D5, D6, D7

char ssid[] = "realme narzo 60x 5G";   // your network SSID (name) 
char pass[] = "h2sdc6cx";   // your network password

WiFiClient  client;

unsigned long startTime = 0;
unsigned int eventCount = 0;
int heartrate=0;
int heartrate_f=0;

unsigned long ChannelNumber = 2081187;


int statusCode = 0;
int field[9] = {0,1,2,3,4,5,6,7,8};

void setup() {
  Serial.begin(115200);      // Initialize serial 
  pinMode(14,INPUT); //LO+ ke liye
  pinMode(12,INPUT); //LO- ke liye
  pinMode(0,OUTPUT); //red ke liye
  pinMode(2,OUTPUT); //green ke liye
  lcd.init();
  lcd.begin(16, 2); // 16x2 LCD module
  lcd.backlight();
  lcd.setCursor(0, 0);
  //lcd.print("Waiting for data...");
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
  WiFi.mode(WIFI_STA);
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    digitalWrite(0,HIGH);
    digitalWrite(2,LOW);
    //Serial.print("Attempting to connect to SSID: ");
   // Serial.println("realme narzo 60x 5G");
    lcd.setCursor(0, 0);
    lcd.print("Attempting to connect to SSID: ");
    lcd.setCursor(0, 1);
    lcd.print("realme narzo 60x 5G");
    delay(2000);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. 
    //  Serial.print(".");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("connecting...");
      delay(5000);     
    } 
   // Serial.println("\nConnected");
    digitalWrite(2,HIGH);
    digitalWrite(0,LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Connected");
    delay(2000);
  }
}
int heart;

void scrollingLCD(String s,int heart)
{
  if(s.length()<=16)
  lcd.print(s);
  else{
    for(int i=0;i<=s.length()-16;i++)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("heart rate: " + String(heart));
      lcd.setCursor(0,1);
      lcd.print(s.substring(i,i+16));
      delayInterrupt(200);
    }
  }
}


int heartratecal(){
   // put your main code here, to run repeatedly:
  if((digitalRead(14)==HIGH)||(digitalRead(12)==HIGH)){
    Serial.print(".");
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
return(heartrate_f);
}

void delayInterrupt(int n)
{
  int x=millis();
  while(millis()-x<n)
    heart=heartratecal();
}


void loop() {
    heart=heartratecal();
    // Read and store all the latest field values, location coordinates, status message, and created-at timestamp
    // use ThingSpeak.readMultipleFields(channelNumber, readAPIKey) for private channels
    statusCode = ThingSpeak.readMultipleFields(ChannelNumber,"8FE7CYIW5NC2COEC");
    
    if(statusCode == 200)
    {
      // Fetch the stored data

      int ageofppl = ThingSpeak.getFieldAsInt(field[1]); // Field 1
      int sex = ThingSpeak.getFieldAsInt(field[2]); // Field 2
      int cp = ThingSpeak.getFieldAsInt(field[3]); // Field 3
      int exng = ThingSpeak.getFieldAsInt(field[5]); // Field 5
      int caa = ThingSpeak.getFieldAsInt(field[6]); // Field 6
      int thal = ThingSpeak.getFieldAsInt(field[7]); // Field 7
      int pred = ThingSpeak.getFieldAsInt(field[8]); // Field 8

     
//      Serial.println("age " + String(ageofppl));
//      Serial.println("sex " + String(sex));
//      Serial.println("cp " + String(cp));
//      Serial.println("heart rate " + String(heart));
//      Serial.println("exng " + String(exng));
//      Serial.println("caa " + String(caa));
//      Serial.println("thal " + String(thal));
//      Serial.println("pred " + String(pred));

      
      
      if(pred==0){
      lcd.clear();
      lcd.setCursor(0, 1);
      scrollingLCD("    safe!!  Risk of heart attack is low !!! :) ",heart );
      }
      else{
      lcd.clear();
      lcd.setCursor(0, 1);
      scrollingLCD("    take precaution!! risk of heart attack is high !!! :(",heart );
      }
delayInterrupt(2000);
    
    
    delayInterrupt(2000);
    
    Serial.println();

    
  // set the fields with the values (write to thingspeak)
  ThingSpeak.setField(1, ageofppl);
  ThingSpeak.setField(2, sex);
  ThingSpeak.setField(3, cp);
  ThingSpeak.setField(4, heart);
  ThingSpeak.setField(5, exng);
  ThingSpeak.setField(6, caa);
  ThingSpeak.setField(7, thal);
  ThingSpeak.setField(8, pred);
 
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(ChannelNumber, "S97I39UFV56KQT2B");
  if(x == 200){
    Serial.println("Channel update successful.");
  }
 
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
    }
      else{
      Serial.println("Problem reading channel. HTTP error code " + String(statusCode)); 
    }
     delayInterrupt(10000); // no need to fetch too often
       
}
