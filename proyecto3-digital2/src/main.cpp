#include <Arduino.h>
#include <Wire.h>
#include<Adafruit_NeoPixel.h>
#define LM75_ADDRESS 0x4C
Adafruit_NeoPixel tira = Adafruit_NeoPixel(8, 15, NEO_GRB + NEO_KHZ800);
void calcular(void);
float temperatura;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  tira.begin();
  tira.show();
  Wire.begin();
}

void loop() {
  tira.setBrightness(50);
  for(int i = 0; i < 8; i++){
    tira.setPixelColor(i, 200, 0, 100);
    tira.show();
    delay(50);
    tira.setPixelColor(i, 0, 0, 0);
    tira.show();
  }
  /*
   if (Serial2.available()){
    int com = Serial2.read();
    if (com == 'm'){
      calcular();
      Serial2.print(temperatura);
    }
  }
  */
}

void calcular(){
  // Solicitar temperatura al LM75
  Wire.beginTransmission(LM75_ADDRESS);
  Wire.write(0x00); // Registro de temperatura (registro 0)
  Wire.endTransmission();
  
  delay(500); // Esperar para dar tiempo al LM75 para que realice la conversión

  // Leer dos bytes de datos de temperatura
  Wire.requestFrom(LM75_ADDRESS, 2);
  if (Wire.available() >= 2) {
    int msb = Wire.read();
    int lsb = Wire.read();

    int rawTemperature = ((msb << 8) | lsb);
    temperatura = (rawTemperature * 0.5) / 128.0;
    
    // Imprimir la temperatura
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println(" °C");
  }
}
