// #include <Arduino.h>

#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   
#include <ArduinoJson.h>

boolean do_once = false;
//Task and Queue handlers
TaskHandle_t Task1; 
TaskHandle_t Task2; 
TaskHandle_t Task3;

static void Tarea1( void * parameter);
static void Tarea2( void * parameter);
static void Tarea3( void * parameter);

bool trn1 = false;
bool trn2 = false;
bool trn3 = false;
int trnact = 0;

void loop() {}
static void Tarea1( void * parameter) {
  while(1){
    trn1 = true;
    trnact=2;
    while(trn2 && trn3 && trnact==1){
      //T0 Esperando
    }
    //Tarea del primer sensor
    trn1 = false;
    vTaskDelay(500 / portTICK_PERIOD_MS); //wait for a second
  }
}
static void Tarea2( void * parameter) {
  while(1){
    trn2 = true;
    trnact=1;
    while(trn1 && trn3 && trnact==0){
      //T1 Esperando
    }
    //Tarea del sensor 2
    trn2 = false;
    vTaskDelay(1000 / portTICK_PERIOD_MS); //wait for a second
  }
}
static void Tarea3( void * parameter) {
  while(1){
    trn3 = true;
    trnact=0;
    while(trn1 && trn2 && trnact==2){
      //T1 Esperando
    }
    Serial.println("T1");
    trn2 = false;
    vTaskDelay(1000 / portTICK_PERIOD_MS); //wait for a second
  }
}


void setup() {
  Serial.begin(115200);
  Serial.println("Running");
  vTaskDelay(1000 / portTICK_PERIOD_MS); //wait for a second

  xTaskCreatePinnedToCore(
    Tarea1,   /* Task function. */
    "Tarea01",     /* name of task. */
    2048,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task1,      /* Task handle to keep track of created task */
    NULL);          /* pin task to core 1 */

  xTaskCreatePinnedToCore(
    Tarea2,   /* Task function. */
    "Tarea02",     /* name of task. */
    2048,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task2,      /* Task handle to keep track of created task */
    NULL);          /* pin task to core 0 */

}
