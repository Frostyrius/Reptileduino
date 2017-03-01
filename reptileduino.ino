// Reptileduino
//
// made by Frostyrius
// https://github.com/Frostyrius

#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define reptileDHTPIN 2
#define roomDHTPIN 3
#define DHTTYPE DHT21

#define heating 4
#define light 5

#define ButtonUp 6
#define ButtonDown 7
#define ButtonBack 8
#define ButtonEnter 9

#define ledPin 13

DHT reptileDHT(reptileDHTPIN, DHTTYPE);
DHT roomDHT(roomDHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

unsigned long currentTime;

float reptileTemp;
float reptileHum;
float roomTemp;
float roomHum;

unsigned long blinkTime = 0;
unsigned int blinkDelay = 1000;
byte ledState = LOW;

unsigned long readTime = 0;
unsigned int readDelay = 2000;

unsigned long lcdTime = 0;
unsigned int lcdDelay = 2000;

void setup() {
  //----------------------- init
  Serial.begin(19200);
  Serial.println("Initiating, please wait...");
  Serial.println("-------------------------");
  pinMode(heating, OUTPUT);
  pinMode(light, OUTPUT);
  pinMode(blinkLED, OUTPUT);
  pinMode(buttonUp, INPUT);
  pinMode(buttonDown, INPUT);
  pinMode(buttonBack, INPUT);
  pinMode(buttonEnter, INPUT);
  lcd.begin(20,4);
  lcd.backlight();
  lcd.clear();
  //----------------------- start screen
  lcd.setCursor(0,0);
  lcd.print("--- ReptileDuino ---");
  lcd.setCursor(0,1);
  lcd.print("------- v0.1 -------");
  lcd.setCursor(0,2);
  lcd.print("-- by  Frostyrius --");
  lcd.setCursor(0,3);
  lcd.print("Frostyrius@gmail.com");
  delay(2000);
  lcd.clear();
  delay(500);
  //----------------------- static texts
  lcd.setCursor(1,0);
  lcd.print("time day to be done ");
  lcd.setCursor(0,1);
  lcd.print("-- Rept ---- Room --");
  lcd.setCursor(0,2);
  lcd.print("Tmp:");
  lcd.setCursor(0,3);
  lcd.print("Hum:");
  lcd.setCursor(10,2);
  lcd.print("Tmp:");
  lcd.setCursor(10,3);
  lcd.print("Hum:");
}

void loop() {
  currentTime = millis();
  
  if(readTime < currentTime){
    //---------------------------------- reading temps + humidities
    reptileTemp = reptileDHT.readTemperature();
    reptileHum = reptileDHT.readHumidity();
    roomTemp = roomDHT.readTemperature();
    roomHum = roomDHT.readHumidity();
    
    readTime = currentTime + readDelay;
  }

  if( lcdTime < currentTime ){
    if( isnan(reptileTemp) || isnan(reptileHum) ){
      //--- reptile DHT read error ----------------------------
      lcd.setCursor(5,2);
      lcd.print("ERR ");
      lcd.setCursor(5,3);
      lcd.print("ERR ");
    }else{
      //--- reptile DHT read ok -------------------------------
      lcd.setCursor(5,2);
      lcd.print(reptileTemp);
      lcd.setCursor(5,3);
      lcd.print(reptileHum);
    }
    if(isnan(reptileTemp) || isnan(reptileHum)){
      //--- room DHT read error ----------------------------
      lcd.setCursor(15,2);
      lcd.print("ERR ");
      lcd.setCursor(15,3);
      lcd.print("ERR ");
    }else{
      //--- room DHT read ok -------------------------------
      lcd.setCursor(15,2);
      lcd.print(roomTemp);
      lcd.setCursor(15,3);
      lcd.print(roomHum);
    }
    lcdTime = currentTime + lcdDelay;
  }
  //--------------------------------- LED blink  
  if( ledTime < currentTime ){
    if(ledState){
      ledState = LOW;
    }else{
      ledState = HIGH;
    }
    digitalWrite(ledPin, ledState);
    ledTime = currentTime + ledDelay;
  }

  //--------------------------------- program end
}
