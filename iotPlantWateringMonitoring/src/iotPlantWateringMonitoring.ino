#include <Arduino.h>

unsigned long readTimeInMinutes = 1;

int ledRed    = 6;
int ledGreen  = 7;

int sensorData = 1024;

unsigned long second = 1000;
unsigned long minute = second * 60;

unsigned long startTime = millis();
unsigned long lastReadTime = startTime;
unsigned long sensorReadDelay = minute * readTimeInMinutes;
unsigned long ledToggleTime = startTime;
unsigned long ledToggleDelayLong = 1000;
unsigned long ledToggleDelayMid = 600;
unsigned long ledToggleDelayShort = 300;

void setup() {
  Serial.begin(9600);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(A0, INPUT);
  delay(second / 2);
  sensorData = analogRead(A0);
  Serial.println(sensorData);
  lastReadTime = millis();
}

void toggleRedLed(unsigned long delay){
  if (!canProcess(delay, ledToggleTime)){
    return;
  }

  ledToggleTime = millis();
  if (digitalRead(ledRed) == LOW){
    redLedOn();
    return;
  }

  ledOff();
}

void redLedOn(){
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledRed, HIGH);
}

void greedLedOn(){
  digitalWrite(ledRed, LOW);
  digitalWrite(ledGreen, HIGH);
}

void ledOff(){
  digitalWrite(ledRed, LOW);
  digitalWrite(ledGreen, LOW);
}

bool canProcess(unsigned long delayValue, unsigned long referenceValue){
  unsigned long now = millis();
  unsigned long compareTO = now - delayValue;
  if (delayValue > now){
    return false;
  }
  
  return (now - delayValue) > referenceValue;
}

void toggleGreenLed(unsigned long delay){
  if (!canProcess(delay, ledToggleTime)){
    return;
  }
  
  ledToggleTime = millis();
  if (digitalRead(ledGreen) == LOW){
    greedLedOn();
    return;
  }
  ledOff();
}

int readMoistureData(){
  if (canProcess(sensorReadDelay, lastReadTime)){
    sensorData = analogRead(A0);
    lastReadTime = millis();
    Serial.println(sensorData);
  }
  
  return sensorData;
}

void handleSensor(){
  int moistureMeasure = readMoistureData();

  if (moistureMeasure >= 1000){
    redLedOn();
    return;
  }

  if (moistureMeasure >= 900){
    toggleRedLed(ledToggleDelayMid);
    return;
  }

  if (moistureMeasure >= 750){
    toggleRedLed(ledToggleDelayShort);
    return;
  }

  if (moistureMeasure < 50){
    toggleGreenLed(ledToggleDelayLong);
  }
}

void loop() {
  handleSensor();
}