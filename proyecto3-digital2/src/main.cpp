#include <Arduino.h>
#include <Wire.h>
#include<Adafruit_NeoPixel.h>
#define LM75_ADDRESS 0x48
Adafruit_NeoPixel tira = Adafruit_NeoPixel(8, 15, NEO_GRB + NEO_KHZ800);
void calcular(void);
float temperatura;
unsigned long tiempoPrevio = 0;  // Almacena el tiempo del último evento
const int intervalo = 10000;      // Intervalo de tiempo en milisegundos (10 segundos)
int com=0;
void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  tira.begin();
  tira.show();
  Wire.begin();
}

void loop() {
//  tira.setBrightness(50);
  //for(int i = 0; i < 8; i++){
//    tira.setPixelColor(i, 200, 0, 100);
  //  tira.show();
    //delay(50);
    //tira.setPixelColor(i, 0, 0, 0);
  //  tira.show();
//  }
  //Obtener el tiempo actual
  unsigned long tiempoActual = millis();
  calcular();
  // Verificar si ha pasado el intervalo de tiempo
  if (tiempoActual - tiempoPrevio >= intervalo) {
    Serial2.println(temperatura);
    // Actualizar el tiempo del último evento
    tiempoPrevio = tiempoActual;
  }
  while (Serial2.available()){
    char com = Serial2.read();
    String mensaje = Serial2.readStringUntil('\n');
    Serial.println(mensaje); //Se imprime el dato en el monitor serial
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
  }
  delay(500);
}
