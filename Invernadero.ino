// LiquidCrystal I2C - Version: Latest 
#include <LiquidCrystal_I2C.h>

#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2     // Pin digital al que esta conectado el sensor DHT
#define DHTTYPE DHT11   // DHT 11
#define FOTORESISTENCIAPIN A0     // Pin analogo al que esta conectado la fotoresistencia
#define SENSORHUMEDADSUELOPIN A1     // Pin analogo al que esta conectado al sensor de humedad del suelo
#define SENSORLLUVIAPIN A2     // Pin analogo al que esta conectado al sensor de lluvia
#define LEDROJOPIN 8     // Pin digital donde esta conectado el led rojo
#define LEDAMARILLOPIN 9     // Pin digital donde esta conectado el led amarillo
#define LEDVERDEPIN 10     // Pin digital donde esta conectado el led verde
#define RELAY1PIN 4     // Pin digital donde esta conectado el ralay1

float humedadAmbiente = 0;
float temperaturaAmbiente = 0;
int valorLuz = 0;
int valorLluvia = 0;
int valorHumedadSuelo = 0;
int valorLEDROJOPIN = 0;
int valorLEDAMARILLOPIN = 0;
int valorLEDVERDEPIN = 0;
int valorRelay1 = 1;

DHT dht(DHTPIN, DHTTYPE);

//Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  Serial.begin(9600);
  pinMode(LEDROJOPIN , OUTPUT);  //definir pin del led rojo como salida
  pinMode(LEDAMARILLOPIN , OUTPUT);  //definir pin del led amarillo como salida
  pinMode(LEDVERDEPIN , OUTPUT);  //definir pin del led verde como salida
  pinMode(RELAY1PIN , OUTPUT);  //definir pin del relay1 como salida

  switchRalay1(0);

  Serial.println(F("Sistemna Riego!"));
  
  // Inicializar el LCD
  lcd.init();
  
  //Encender la luz de fondo.
  lcd.backlight();
  
  // Escribimos el Mensaje en el LCD.
  lcd.print("Sistema Riego");
  escribirLCD2("Cargando.");
  delay(1000);
  escribirLCD2("Cargando..");
  delay(1000);
  escribirLCD2("Cargando...");
  delay(1000);
  escribirLCD2("Cargando....");
  delay(1000);
  

  dht.begin();
}

void loop() {
  
    //Realizando prueba de sensores cada 5 segundos
    pruebaSensores();
    estaLloviendo();
    tierraHumeda();
    encenderBomba();
    delay(2000);
}

void encenderBomba(){
//  if(valorLuz<=300 && valorHumedadSue/lo>=500 && valorLluvia>=900 && temperaturaAmbiente>=30.00){
  if(valorLuz<=300 && valorHumedadSuelo>=500 && valorLluvia>=500){
      switchLEDVERDEPIN(1);
      switchRalay1(1);
      Serial.println("Bomba Encendida");
  }else{
      switchLEDVERDEPIN(0);
      switchRalay1(0);
      Serial.println("Bomba Apagada");
  }

  Serial.println();
  
}

void tierraHumeda(){
  if(valorHumedadSuelo<=500){
      switchLEDROJOPIN(0);
      Serial.println("Tierra mojada");
  }else{
      switchLEDROJOPIN(1);
      Serial.println("Tierra seca");
  }
}

void estaLloviendo(){
  if(valorLluvia<=500){
      switchLEDAMARILLOPIN(1);
      Serial.println("Esta lloviendo");
  }else{
      switchLEDAMARILLOPIN(0);
      Serial.println("No esta lloviendo");
  }
}

void pruebaSensores(){
  Serial.println("--- Realizando prueba de sensores ---");
  lecturasDHT();
  lecturaLuz();
  lecturaLluvia();
  lecturaHumedadSuelo();
  Serial.println();
  
  escribirLCD1("Temp:" + String(temperaturaAmbiente,0) + "C" + "Luz:"+crearPorcentajeLuz(valorLuz));
  escribirLCD2("Hum.Suelo:"+crearPorcentajeHumedad(valorHumedadSuelo));
}

String crearPorcentajeLuz(int valor){
  // float moisture_percentage = ( 100 - ( (valor/100.00) * 1023 ) );
  float moisture_percentage = map (valor, 0, 1023, 0, 100);

  return String(moisture_percentage,0)+"%";
}

String crearPorcentajeHumedad(int valor){
  float moisture_percentage = ( 100 - ( (valor/1023.00) * 100 ) );
  return String(moisture_percentage,0)+"%";
}

void switchRalay1(int val){
  digitalWrite(RELAY1PIN, !val);
}

void escribirLCD1(String text){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(text);
}

void escribirLCD2(String text){
  // lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(text);
}

void pruebaLcd(){
   // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
  lcd.setCursor(0, 1);
   // Escribimos el número de segundos trascurridos
  lcd.print(millis()/1000);
  lcd.print(" Segundos");
  delay(100);
}

void secuenciaLed(){
  switchLEDROJOPIN(1);
  switchLEDAMARILLOPIN(0);
  switchLEDVERDEPIN(0);
  delay(500);
  switchLEDROJOPIN(0);
  switchLEDAMARILLOPIN(1);
  switchLEDVERDEPIN(0);
  delay(500);
  switchLEDROJOPIN(0);
  switchLEDAMARILLOPIN(0);
  switchLEDVERDEPIN(1);
  delay(500);
}

void switchLEDROJOPIN(int val){
  digitalWrite(LEDROJOPIN, val);
}

void switchLEDAMARILLOPIN(int val){
  digitalWrite(LEDAMARILLOPIN, val);
}

void switchLEDVERDEPIN(int val){
  digitalWrite(LEDVERDEPIN, val);
}

void lecturaHumedadSuelo(){
  //Leemos el valor del sensor de lluvia
  valorHumedadSuelo = analogRead(SENSORHUMEDADSUELOPIN);
  Serial.println("Humedad suelo: " + String(valorHumedadSuelo));
  // escribirLCD1("Humedad suelo:");
  // escribirLCD2(String(valorHumedadSuelo));
  // delay(2000);
}

void lecturaLluvia(){
  //Leemos el valor del sensor de lluvia
  valorLluvia = analogRead(SENSORLLUVIAPIN);
  Serial.println("Lluvia: " + String(valorLluvia));
  // escribirLCD1("Lluvia");
  // escribirLCD2(String(valorLluvia));
  // delay(2000);
}

void lecturaLuz(){
  //Leemos el valor de la fotoresistencia
  valorLuz = analogRead(FOTORESISTENCIAPIN);
  // Serial.println("Luz: ");
  Serial.println("Luz: " + String(valorLuz));
  // escribirLCD1("Luz");
  // escribirLCD2(String(valorLuz));
  // delay(2000);
  
}

void lecturasDHT(){
  // delay(2000);
  
  // float h = dht.readHumidity();
  // float t = dht.readTemperature();
  
  humedadAmbiente = dht.readHumidity();
  temperaturaAmbiente = dht.readTemperature();


  // Comprueba si alguna lectura falló
  if (isnan(humedadAmbiente) || isnan(temperaturaAmbiente)) {
    Serial.println(F("Error al conectar al sensor DHT!"));
    return;
  }
  
  Serial.println("Humedad ambiente: " + String(humedadAmbiente) + "%");
  Serial.println("Temperatura ambiente: " + String(temperaturaAmbiente) + "°C");
  // escribirLCD1("Humedad Ambiente");
  // escribirLCD2(String(humedadAmbiente));
  // delay(2000);
  // escribirLCD1("Temp Ambiente");
  // escribirLCD2(String(humedadAmbiente) + "°C");
  // delay(2000);
  
}