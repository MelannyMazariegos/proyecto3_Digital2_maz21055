//Se incluyen librerias
#include <Arduino.h>
#include <Wire.h>
#include<Adafruit_NeoPixel.h>
//Direccion del sensor para la comunicacion I2C
#define LM75_ADDRESS 0x48
Adafruit_NeoPixel tira = Adafruit_NeoPixel(8, 15, NEO_GRB + NEO_KHZ800);
//Prototipos de funciones
void calcular(void);
void medicion(void);
void tarjeta(void);
float temperatura;
unsigned long tiempoPrevio = 0;  // Almacena el tiempo del último evento
const int intervalo = 10000;      // Intervalo de tiempo en milisegundos (10 segundos)
int com=0;
void setup() {
  //Se inicializan los monitores seriales
  Serial.begin(115200);
  Serial2.begin(115200);
  tira.begin();
  tira.show();
  Wire.begin();
}

void loop() {
  tira.setBrightness(50); //Brillo del neopixel
  tira.setPixelColor(0, 200, 0, 100); //Color del primer pixel
  tira.show();
  //Obtener el tiempo actual
  unsigned long tiempoActual = millis();
  calcular();
  // Verificar si ha pasado el intervalo de tiempo
  if (tiempoActual - tiempoPrevio >= intervalo) {
    Serial2.println(temperatura); //Se envia la medicion
    // Actualizar el tiempo del último evento
    tiempoPrevio = tiempoActual;
  }
  while (Serial2.available()){
    //Se lee el caracter que manda el tiva dependiendo de la accion que se este realizando
    char com = Serial2.read();
    String comando = String(com); //Se convierte en caracter en texto
    if (comando == "1"){
      //Funcion para el patron de neopixel cuando se manda una medicion
      medicion();
    }
    else if (comando == "2"){
      //Funcion para el patron de neopixel cuando se guarda una medicion
      tarjeta();
    }
    String mensaje = Serial2.readStringUntil('\n');
    Serial.println(mensaje); //Se imprime la medicion en el monitor serial
  }
  
}

void calcular(){
  // Solicitar temperatura al LM75
  Wire.beginTransmission(LM75_ADDRESS);
  Wire.write(0x00); // Registro de temperatura
  Wire.endTransmission();
  
  delay(500); // Esperar para dar tiempo al LM75 para que realice la conversión

  // Leer dos bytes de datos de temperatura
  Wire.requestFrom(LM75_ADDRESS, 2);
  if (Wire.available() >= 2) {
    int msb = Wire.read();
    int lsb = Wire.read();
    //Se calcula la temperatura en grados centigrados
    int rawTemperature = ((msb << 8) | lsb);
    temperatura = (rawTemperature * 0.5) / 128.0;
  }
  delay(500);
}
void medicion(){
  for(int i = 0; i < 8; i++){
    //Secuencia de leds para cuando se manda un dato
    tira.setPixelColor(i, 50, 150, 100);
    tira.show();
    delay(100);
    tira.setPixelColor(i, 0, 0, 0);
    tira.show();
  }
}
void tarjeta(){
  //Secuencia de leds para cuando se guarda un dato
  for(int i = 0; i < 8; i++){
    tira.setPixelColor(i, 150, 200, 0);
    tira.show();
    delay(100);
    tira.setPixelColor(i, 0, 0, 0);
    tira.show();
  }
}