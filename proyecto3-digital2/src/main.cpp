#include <Arduino.h>
#include <Wire.h>

#define LM75_ADDRESS 0x4C

void calcular(void);
float temperatura;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  Wire.begin();
}

void loop() {
   if (Serial2.available()){
    int com = Serial2.read();
    if (com == 'm'){
      calcular();
      Serial2.print(temperatura);
    }
  }
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
