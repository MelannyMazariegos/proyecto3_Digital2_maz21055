#include <Arduino.h>
#include <Wire.h>
#define LM75_ADDRESS 0x4C
void setup() {
  Serial.begin(115200);
  Wire.begin();
  //Serial2.begin(115200);
}

void loop() {
  // Solicitar temperatura al LM75
  Wire.beginTransmission(LM75_ADDRESS);
  Wire.write(0); // Registro de temperatura (registro 0)
  Wire.endTransmission();
  
  delay(500); // Esperar para dar tiempo al LM75 para que realice la conversión

  // Leer dos bytes de datos de temperatura
  Wire.requestFrom(LM75_ADDRESS, 2);
  if (Wire.available() >= 2) {
    int msb = Wire.read();
    int lsb = Wire.read();

    // Calcular la temperatura en grados Celsius
    int temp = ((msb << 8) | lsb) >> 7;
    
    // Imprimir la temperatura
    Serial.print("Temperatura: ");
    Serial.print(temp);
    Serial.println(" °C");
  }

  delay(1000); // Esperar un segundo antes de la próxima lectura
}