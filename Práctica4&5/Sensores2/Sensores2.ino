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

const char* ssid = "ARRIS-FC92";
const char* password = "2EFC9145F2429281";
//const char* ssid = "Kassandra";
//const char* password = "12345678";

#define BOTtoken "6462574155:AAFY7yrdXGU2MtLYX3asiS2KDy7IK7fbBdQ"  
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
bool tarea1, tarea2, tarea3 = false;
int turno = 1;
bool cancelar = false;

//Pines para los sensores
int PinSonido = 4;
int LEDS = 19;
int PinInfrarrojo = 5;
int LEDI = 18;
//Mensaje de la uart
char mensaje=' ';
int LEDU = 21;


//Tarea del sensor infrarrojo
static void Tarea1( void * parameter) {
  while(!cancelar){
    tarea1 = true;
    while(tarea2 && tarea3 && !cancelar){
      if (turno==1){
        tarea1=false;
          while (turno==1 && !cancelar){
            /*espera*/
          }
          tarea1=true;
      }
    }
    if (cancelar) break;
    //Ejecuta la sección crítica
    turno=1;
    tarea2=false;
    tarea3=false;
  }
}
//Tarea del sensor de sonido
static void Tarea2( void * parameter) {
  while(!cancelar){
    tarea2 = true;
    while(tarea1 && tarea3 && !cancelar){
      if (turno==2){
        tarea2=false;
          while (turno==2 && !cancelar){
            /*espera*/
          }
          tarea2=true;
      }
    }
    if (cancelar) break;
    //Ejecuta la sección crítica
    turno=2;
    tarea1=false;
    tarea3=false;
  }
}
static void Tarea3( void * parameter) {
  while(!cancelar){
    tarea3 = true;
    while(tarea2 && tarea3 && !cancelar){
      if (turno==3){
        tarea3=false;
          while (turno==3 && !cancelar){
            /*espera*/
          }
          tarea3=true;
      }
    }
    if (cancelar) break;
    //Ejecuta la sección crítica
    turno=3;
    tarea1=false;
    tarea2=false;
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
    int valorI = digitalRead(PinInfrarrojo);
    digitalWrite(LEDI, valorI);
    }
    
    if (text == "/led_Sonido") {
      bot.sendMessage(chat_id, "Turno del sensor de Sonido", "");
    int valorS = digitalRead(PinSonido);
    digitalWrite(LEDS, valorS);
    }
    
    if (text == "/led_UART") {
      bot.sendMessage(chat_id, "Turno de la tarea en la UART", "");
      bot.sendMessage(chat_id, "Presione 'a' para encender el led y 'b' para apagarlo", "");
      if (Serial.available() !=0){
        mensaje=Serial.read();
        if(mensaje=='a'){
         digitalWrite(LEDU, HIGH);
         bot.sendMessage(chat_id, "Se introdujo la letra a", "");
       }
        else if (mensaje=='b'){
          digitalWrite(LEDU, LOW);
          bot.sendMessage(chat_id, "Se introdujo la letra b", "");
        }
    }
    }
  }
}

void setup() {
  pinMode(LEDI, OUTPUT);
  pinMode(LEDS, OUTPUT);
  pinMode(LEDU, OUTPUT);
  pinMode(PinInfrarrojo, INPUT);
  pinMode(PinSonido, INPUT);
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS); //wait for a second
  xTaskCreatePinnedToCore(Tarea1,"Tarea01",2048,NULL,1,&Task1,NULL);
  xTaskCreatePinnedToCore(Tarea2,"Tarea02",2048,NULL,1,&Task2,NULL);
  xTaskCreatePinnedToCore(Tarea3,"Tarea03",2048,NULL,1,&Task3,NULL);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      
    client.setTrustAnchors(&cert); 
  #endif
  
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

