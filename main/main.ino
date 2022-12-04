#include <SoftwareSerial.h>

#define R 10000
#define V 5.00
#define Ro 10000
#define coefKelvin 273.15
#define T0 (25.00 + 273.15)

int analogValue; 
float Temp, TC, TF, TK;

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

void initTemperatureFromAnalog() {
  analogValue = analogRead(thermistorPin);
  Temp = log(((1024.00 * Ro/analogValue) - Ro));
  Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
  TK = Temp;
  TC = TK - coefKelvin;
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