/*
ESPboy Digital Thermometer with DS18B20 sensor
for www.ESPboy.com project by RomanS
v1.0
*/

#include "ESPboyInit.h"
#include "ESPboyLED.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D4
#define DELAY_BETWEEN_READS 500


ESPboyInit myESPboy;
ESPboyLED myLED;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress addr;

int16_t tempas[128];

void setup(){
  Serial.begin(115200);
  
  myESPboy.begin("Digital Thermometer");
  myLED.begin(&myESPboy.mcp);

  sensors.begin();
  if (!sensors.getDeviceCount()){ 
    myESPboy.tft.setTextColor(TFT_RED);
    myESPboy.tft.setTextSize(1);
    myESPboy.tft.drawString(F("Module not found"),16,120);
    while (1) delay(100);
  }
  sensors.getAddress(addr, 0);
  sensors.setResolution(addr, 12);  

  delay(500);
  sensors.requestTemperatures();
  int16_t tempa = sensors.getTempCByIndex(0)+0.6;
  for (uint8_t i=0; i<128; i++) tempas[i]=tempa;
}


void loop(){
  static float tempa;
  static uint8_t i; 
  static uint32_t valAver;

  sensors.requestTemperatures();
  tempa = sensors.getTempCByIndex(0)+0.6;

  for (i=1; i<128; i++)
    myESPboy.tft.drawPixel(i,128-((tempas[i]-valAver)+44), TFT_BLACK);  
  
  memmove(&tempas[0], &tempas[1], 127*sizeof(tempas[0]));
  tempas[127] = tempa;

  valAver=0;
  for(i=100; i<128; i++) 
    valAver+=tempas[i];
  valAver/=27;

  for (i=0; i<128; i++)
    myESPboy.tft.drawPixel(i,128-((tempas[i]-valAver)+44), TFT_GREEN);  

  myESPboy.tft.drawRect(0,43,128,128-43,TFT_BLUE);
  
  myESPboy.tft.setTextColor(TFT_WHITE, TFT_BLACK);
  myESPboy.tft.setTextSize(1);
  myESPboy.tft.drawString(F("Temperature Celsius"),0,0);

  if(tempa<0) myESPboy.tft.setTextColor(TFT_BLUE, TFT_BLACK);
  if(tempa>=0) myESPboy.tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  if(tempa>36 && tempa<37) myESPboy.tft.setTextColor(TFT_GREEN, TFT_BLACK);
  if(tempa>37) myESPboy.tft.setTextColor(TFT_RED, TFT_BLACK);
  
  myESPboy.tft.setTextSize(3);
   myESPboy.tft.drawString("       ",0,14);
  myESPboy.tft.drawString((String)tempa,0,14);
  
  delay(DELAY_BETWEEN_READS);  
}
