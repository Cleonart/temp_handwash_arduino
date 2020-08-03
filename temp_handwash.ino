#include <Arduino.h>
#include <TM1637Display.h>
#include "DHT.h"

// Module connection pins (Digital Pins)
#define CLK 4
#define DIO 3
#define DHTPIN 9
#define DHTTYPE DHT11 

// The amount of time (in milliseconds) between tests
#define TEST_DELAY   2000

int k;
uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };

int num_state_now = 0;

const uint8_t SEG_DONE[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d1
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_C | SEG_E | SEG_G,                           // n
  SEG_A | SEG_F | SEG_E | SEG_D             // E
  };

TM1637Display display(CLK, DIO);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  display.setBrightness(0x0f);
  pinMode(2, INPUT_PULLUP);
}

void loop() {
  float t = dht.readTemperature();
  Serial.println(t);
  showDecimal(t);
  delay(2000);
  int L = digitalRead(2);
  Serial.println(L);
  //updateDisplay(8230);
}

void updateDisplay(int num){
  
  String numberInString = String(num);
  
  if(num < 10){
     data[0] = display.encodeDigit(numberInString[0]);
     data[1] = 0x00;
     data[2] = 0x00;
     data[3] = 0x00;
  }
  
  else if(num >= 10 && num <= 99){
     data[0] = display.encodeDigit(numberInString[0]);
     data[1] = display.encodeDigit(numberInString[1]);
     data[2] = 0x00;
     data[3] = 0x00;
  }
  
  else if(num >= 100 && num <= 999){
     data[0] = display.encodeDigit(numberInString[0]);
     data[1] = display.encodeDigit(numberInString[1]);
     data[2] = display.encodeDigit(numberInString[2]);
     data[3] = 0x00;
  }
  
  else if(num >= 1000){
     data[0] = display.encodeDigit(numberInString[0]);
     data[1] = display.encodeDigit(numberInString[1]);
     data[2] = display.encodeDigit(numberInString[2]);
     data[3] = display.encodeDigit(numberInString[3]);
  }

  // only update if before and after is not the same
  display.setSegments(data);
  delay(TEST_DELAY);  
}

void showDecimal(int num){
  String numberInString = String(num);
  data[0] = display.encodeDigit(numberInString[0]);
  data[1] = display.encodeDigit(numberInString[1]);
  data[2] = SEG_A | SEG_F | SEG_E | SEG_D;
  data[3] = 0x00;
  display.setSegments(data);
  delay(TEST_DELAY);  
}
