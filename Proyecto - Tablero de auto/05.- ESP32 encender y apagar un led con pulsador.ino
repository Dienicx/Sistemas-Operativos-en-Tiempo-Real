
//WOKWI: https://wokwi.com/projects/384862025431302145

#if CONFIG_FREERTOS_UNICROE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif
#include<ESP32Servo.h>
Servo Servo;
static TaskHandle_t Tarea01 = NULL; //Tarea Puertas abiertas
static TaskHandle_t Tarea02 = NULL; //Tarea Cajuela
static TaskHandle_t Tarea03 = NULL; //Tarea Capó
static TaskHandle_t Tarea04 = NULL; //Tarea Temperatura
static TaskHandle_t Tarea05 = NULL; //Tarea Intermitentes (activar al mismo tiempo sólo con boton rojo)
static TaskHandle_t Tarea06 = NULL; //Tarea Presión de aceite
static TaskHandle_t Tarea07 = NULL; //Tarea Control crucero
static TaskHandle_t Tarea08 = NULL; //Tarea  Bolsas de aire

#define LED_PIN 2 //Testigo de puertas abiertas
#define LED_PIN2 15 //Testigo de cajuela abierta
#define LED_PIN3 17 //Testigo de capó abierto
#define LED_PIN4 5 //Testigo de Temperatura
#define LED_PIN5 18 //Intermitente izquierda
#define LED_PIN6 19 //Intermitente derecha
#define LED_PIN7 21 //Testigo de nivel de aceite
#define LED_PIN8 22 //Testigo de control crucero
#define Servogasolina 16 //Medidor de gasolina
#define BUTTON_PIN 13 //Sensor de puertas abiertas
#define BUTTON_PIN2 14 //Sensor de cajuela abierta
#define BUTTON_PIN3 12 //Sensor de capó abierto
#define BUTTON_PIN4 27 //Palanca de intermitente izquierda
#define BUTTON_PIN5 26 //Palanca de intermitente derecha
#define BUTTON_PIN6 25 //Boton de estacionamiento (intermitente izquierda y derecha a la vez)
#define BUTTON_PIN7 23 //Sensor del control crucero


//tarea 1 Puertas abiertas(se activa al presionar boton 1)
void Tarea1(void *parameter) {
while(1){  
if (digitalRead(BUTTON_PIN) == HIGH){
delay(10);
digitalWrite(LED_PIN, HIGH);
 }
else{
  delay(10);
  digitalWrite(LED_PIN, LOW);
  }
 }
}

//tarea 2 Cajuela(se activa al presionar boton 2)
 void Tarea2(void *parameter) {
 while(1){  
 if (digitalRead(BUTTON_PIN2) == HIGH){
  delay(10);
 digitalWrite(LED_PIN2, HIGH);
  }
 else{
  delay(10);
   digitalWrite(LED_PIN2, LOW);
  }
 }
}

//tarea 3 Capó(se activa al presionar boton 3)
 void Tarea3(void *parameter) {
 while(1){  
 if (digitalRead(BUTTON_PIN3) == HIGH){
  delay(10);
 digitalWrite(LED_PIN3, HIGH);
  }
 else{
  delay(10);
   digitalWrite(LED_PIN3, LOW);
  }
 }
}

//tarea 4 Temperatura(intensidad del led depende del potenciómetro 1. El testigo es como en la sig. imagen)
 void Tarea4(void *parameter) {//https://gmb.net/es/wp-content/uploads/sites/2/2021/07/Sensor-Temp-FLECHA.jpg
 while(1){  
  int Temperatura = analogRead(34); //Sensor de temperatura
  int intensidad = map(Temperatura,0,4095,0,255);
  analogWrite(LED_PIN4,intensidad);
  }
 }

//tarea 5 Intermitentes (Boton 4 para izquierda, boton 5 para derecha. activar al mismo tiempo sólo con boton rojo)
 void Tarea5(void *parameter) {
 while(1){
 if (digitalRead(BUTTON_PIN4) == HIGH){
  delay(10);
  digitalWrite(LED_PIN6, HIGH);
  delay(250);                       
  digitalWrite(LED_PIN6, LOW);
  delay(250);   
  }
 else{
  delay(10);
   digitalWrite(LED_PIN6, LOW);
  } 

if (digitalRead(BUTTON_PIN5) == HIGH){
  delay(10);
  digitalWrite(LED_PIN5, HIGH);
  delay(250);                       
  digitalWrite(LED_PIN5, LOW);
  delay(250);   
  }
 else{
  delay(10);
   digitalWrite(LED_PIN5, LOW);
  } 

if (digitalRead(BUTTON_PIN6) == HIGH){
  delay(10);
  digitalWrite(LED_PIN5, HIGH);
  digitalWrite(LED_PIN6, HIGH);
  delay(250);                       
  digitalWrite(LED_PIN5, LOW);
  digitalWrite(LED_PIN6, LOW);
  delay(250);      
  }
 else{
  delay(10);
   digitalWrite(LED_PIN5, LOW);
   digitalWrite(LED_PIN6, LOW);
  } 
 }
}

//tarea 6 Presión de aceite (led se activa cuando val analógico del potenciómetro 2 es <1000)
 void Tarea6(void *parameter) {
 while(1){  
  int Aceite = analogRead(33);
  if (analogRead(33) <= 1000){
  delay(10);
  digitalWrite(LED_PIN7, HIGH);
  }
 else{
  delay(10);
   digitalWrite(LED_PIN7, LOW);
  } 
  }
 }

//tarea 7 Control crucero(se activa al presionar boton 6)
 void Tarea7(void *parameter) {
 while(1){  
 if (digitalRead(BUTTON_PIN7) == HIGH){
  delay(10);
 digitalWrite(LED_PIN8, HIGH);
  }
 else{
  delay(10);
   digitalWrite(LED_PIN8, LOW);
  }
 }
}

//tarea 8 Nivel de gasolina (Servo depende del valor analógico del potenciómetro 3)
 void Tarea8(void *parameter) {
 while(1){  
  int Gasolina = analogRead(4); //Sensor de gasolina
  Serial.println(Gasolina);
  int SensorGasolina = map(Gasolina,0,4095,0,180);
  Servo.write(SensorGasolina);
  delay(15);
 }
}

void setup(){
pinMode(LED_PIN, OUTPUT);
pinMode(LED_PIN2, OUTPUT);
pinMode(LED_PIN3, OUTPUT);
pinMode(LED_PIN4, OUTPUT);
pinMode(LED_PIN5, OUTPUT);
pinMode(LED_PIN6, OUTPUT);
pinMode(LED_PIN7, OUTPUT);
pinMode(LED_PIN8, OUTPUT);


pinMode(BUTTON_PIN, INPUT);
pinMode(BUTTON_PIN2, INPUT);
pinMode(BUTTON_PIN3, INPUT);
pinMode(BUTTON_PIN4, INPUT);
pinMode(BUTTON_PIN5, INPUT);
pinMode(BUTTON_PIN6, INPUT);
pinMode(BUTTON_PIN7, INPUT);

Servo.attach(Servogasolina);
Servo.write(0);



Serial.begin(9600);
  xTaskCreatePinnedToCore(
              Tarea1, //funcion llamda
              "Tarea1", //nombre
              1024,
              NULL,
              1,
              &Tarea01,
              app_cpu);

 Serial.begin(9600);
   xTaskCreatePinnedToCore(
               Tarea2, //funcion llamda
               "Tarea2", //nombre
               1024,
               NULL,
               1,
               &Tarea02,
               app_cpu);

 Serial.begin(9600);
   xTaskCreatePinnedToCore(
               Tarea3, //funcion llamda
               "Tarea3", //nombre
               1024,
               NULL,
               1,
               &Tarea03,
               app_cpu);

 Serial.begin(9600);
   xTaskCreatePinnedToCore(
               Tarea4, //funcion llamda
               "Tarea4", //nombre
               1024,
               NULL,
               1,
               &Tarea04,
               app_cpu);
      
 Serial.begin(9600);
   xTaskCreatePinnedToCore(
               Tarea5, //funcion llamda
               "Tarea5", //nombre
               1024,
               NULL,
               1,
               &Tarea05,
               app_cpu);

 Serial.begin(9600);
   xTaskCreatePinnedToCore(
               Tarea6, //funcion llamda
               "Tarea6", //nombre
               1024,
               NULL,
               1,
               &Tarea06,
               app_cpu);

 Serial.begin(9600);
   xTaskCreatePinnedToCore(
               Tarea7, //funcion llamda
               "Tarea7", //nombre
               1024,
               NULL,
               1,
               &Tarea07,
               app_cpu);

 Serial.begin(9600);
   xTaskCreatePinnedToCore(
               Tarea8, //funcion llamda
               "Tarea8", //nombre
               1024,
               NULL,
               1,
               &Tarea08,
               app_cpu);
}

void loop(){
  
}