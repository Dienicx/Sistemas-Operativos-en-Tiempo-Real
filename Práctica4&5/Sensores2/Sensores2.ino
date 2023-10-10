//Cosas para la conexión del esp con wifi:
// #include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   
#include <ArduinoJson.h>

//const char* ssid = "ARRIS-FC92";
//const char* password = "2EFC9145F2429281";
const char* ssid = "Kassandra";
const char* password = "12345678";

#define BOTtoken "5838117904:AAFEjQ6jywGMqoeYnQmTp0Bik99LZabojg4"  
#define CHAT_ID "6079647597"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int botRequestDelay = 500;
unsigned long lastTimeBotRan;

//Tareas
TaskHandle_t Task1; 
TaskHandle_t Task2; 
TaskHandle_t Task3;
static void Tarea1( void * parameter);
static void Tarea2( void * parameter);
static void Tarea3( void * parameter);

//Variables para los turnos de las tareas
volatile bool tarea[3] = {false,false,false};
int turno = 0;

bool valorI = LOW;
bool valorS = LOW;

//Pines para los sensores
int PinSonido = 4;
int LEDS = 19;
int PinInfrarrojo = 5;
int LEDI = 18;
//Mensaje de la uart
String mensaje;
int LEDU = 21;


//Tarea del sensor infrarrojo
static void Tarea1( void * parameter) {
  while(1){
    //Solicita entrar a la sección crítica
    tarea[0] = true;
    turno = 0;
    while (tarea[1] || tarea[2] || (turno != 0)){
      //Espera a que sea su turno
      vTaskDelay(250);
    }
    //Ejecuta la sección crítica
    tarea[0]=false;
    vTaskDelay(1000 / portTICK_PERIOD_MS); //wait for a second
  }
}
//Tarea del sensor de sonido
static void Tarea2( void * parameter) {
  while(1){
    //Solicita entrar a la sección crítica
    tarea[1] = true;
    turno = 1;
    while (tarea[0] || tarea[2] || (turno != 1)){
      //Espera a que sea su turno
      vTaskDelay(250);
    }
    //Ejecuta la sección crítica
    tarea[1]=false;
    vTaskDelay(1000 / portTICK_PERIOD_MS); //wait for a second
  }
}
static void Tarea3( void * parameter) {
  while(1){
    //Solicita entrar a la sección crítica
    tarea[2] = true;
    turno = 2;
    while (tarea[0] || tarea[1] || (turno != 2)){
      //Espera a que sea su turno
      vTaskDelay(250);
    }
    //Ejecuta la sección crítica
    tarea[2]=false;
    vTaskDelay(1000 / portTICK_PERIOD_MS); //wait for a second
  }
}

//Sección crítica
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Bienvenido, " + from_name + ".\n";
      welcome += "Utilice los siguientes comando para interactuar con el ESP32.\n\n";
      welcome += "/led_Infrarrojo para acceder a la tarea del sensor Infrarrojo \n";
      welcome += "/led_Sonido para acceder a la tarea del sensor de Sonido \n";
      welcome += "/led_UART para acceder a la tarea de la UART \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/led_Infrarrojo") {
      bot.sendMessage(chat_id, "Turno del sensor Infrarrojo", "");
    valorI = digitalRead(PinInfrarrojo);
    digitalWrite(LEDI, valorI);
    delay(2000);
    digitalWrite(LEDI, LOW);
    }
    
    if (text == "/led_Sonido") {
      bot.sendMessage(chat_id, "Turno del sensor de Sonido", "");
    valorS = digitalRead(PinSonido);
    digitalWrite(LEDS, valorS);
    delay(2000);
    digitalWrite(LEDS, LOW);
    }
    
    if (text == "/led_UART") {
      bot.sendMessage(chat_id, "Turno de la tarea en la UART", "");
      bot.sendMessage(chat_id, "Introduzca la letra A para encender el led", "");
      if (Serial.available() > 0){
        char mensaje = Serial.read();
         if (mensaje == 'A') digitalWrite(LEDU, HIGH);
         delay(2000);
         digitalWrite(LEDU, LOW);
      }
    }
  }
}

void setup() {
  Serial.begin(115200);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      
    client.setTrustAnchors(&cert); 
  #endif

  pinMode(LEDI, OUTPUT);
  pinMode(LEDS, OUTPUT);
  pinMode(LEDU, OUTPUT);
  pinMode(PinInfrarrojo, INPUT);
  pinMode(PinSonido, INPUT);
  digitalWrite(LEDI, valorI);
  digitalWrite(LEDS, valorS);
  digitalWrite(LEDU, LOW);

  vTaskDelay(1000 / portTICK_PERIOD_MS); //wait for a second
  xTaskCreatePinnedToCore(Tarea1,"Tarea01",2048,NULL,1,&Task1,NULL);
  xTaskCreatePinnedToCore(Tarea2,"Tarea02",2048,NULL,1,&Task2,NULL);
  xTaskCreatePinnedToCore(Tarea3,"Tarea03",2048,NULL,1,&Task3,NULL);
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); 
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}

