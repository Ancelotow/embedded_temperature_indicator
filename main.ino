#include <SoftwareSerial.h>

#define resistance 10000
#define voltage 5.00
#define B 3977
#define T0 (25.00 + 273.15)

int analogValue; 
float voltageValue, resistanceValue, ln, TC, TK, TF, RT;

int txPin = 2;
int rxPin = 3;
int thermistorPin = A0;
SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);

void setup() {
  Serial.begin(9600);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  mySerial.begin(9600);
}

char* floatToStr(float value) {
  int tmpInt1 = value;
  float tmpFrac = value - tmpInt1;
  int tmpInt2 = trunc(tmpFrac * 100);
  char *str = NULL;
  str = malloc(sizeof(char) * 10);
  sprintf(str, "%d.%d", tmpInt1, tmpInt2);
  return str;
}

float initTemperatureFromAnalog() {
  analogValue = analogRead(thermistorPin);
  voltageValue = analogValue * (voltage / 1023.00);
  resistanceValue = voltage - voltageValue;
  RT = voltageValue / (resistanceValue / resistance);
  ln = log(RT / resistance);
  TK = (1 / ((ln / B) + (1 / T0)));
  TC = TK - 273.15;
  TF = (TC * 1.8) + 32;
}

void sendTemperature() {
  char* strTC = floatToStr(TC);
  char* strTK = floatToStr(TK);
  char* strTF = floatToStr(TF);
  char json[300];
  sprintf(json, "{\"temperature\":{\"celsius\":%s,\"kelvin\":%s,\"farheineit\":%s}}", strTC, strTK, strTF);
  free(strTC);
  free(strTK);
  free(strTF);  
  mySerial.write(json);
}

void loop() {
  initTemperatureFromAnalog();
  sendTemperature();  
  delay(1000);
}