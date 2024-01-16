#if CONFIG_FREERTOS_UNICROE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

#include "BluetoothSerial.h"

static TaskHandle_t Motores = NULL; //Tarea Para el movimiento de motores y evación de paredes
static TaskHandle_t PWM = NULL; //Tarea simula el mecanismo de recolección de basura 
static TaskHandle_t BT = NULL; //Tarea de bluetooth y simulador de nivel de batería
static TaskHandle_t Alerta = NULL; //Tarea de alerta si se atora
//-------------------------------------Pines de los motores
#define Motorrojo 26 
#define MotorNaranja 27 
#define Motoramarillo 14 
#define Motorverde 12 
//-------------------------------------Pines de los ledsitos
#define ledpwm 25
#define ledalerta 32
//-------------------------------------Pines Echo y Trigger del ultrasónico, definición de variables utilizadas por él.
const int Trigger = 33;   
const int Echo = 39;   
long t; //timepo que demora en llegar el eco
long d; //distancia en centimetros
//-------------------------------------Variables para el contador de tiempo límite
bool tiempoExpirado = false;
unsigned long tiempoLimite;
const unsigned long tiempoUso = 60000; //1 minuto
//-------------------------------------Variables para el contador para detectar si se atoró.
bool detenerPorTiempoExcedido = false;
unsigned long tiempoIntentoInicio = 0;
const unsigned long tiempoIntentoMaximo = 4000; // 2 segundos
//-------------------------------------Objeto BluetoothSerial para la comunicación Bluetooth
BluetoothSerial SerialBT; 



//---------------------------------------------------------------Contar tiempecillo límite
void iniciarTemporizador() {
  tiempoLimite = millis() + tiempoUso;
}

// Función para verificar si el tiempo ha expirado
void verificarTiempo() {
  if (millis() > tiempoLimite) {
    tiempoExpirado = true;
  }
}
//---------------------------------------------------------------Modos de movimiento
void detenerMotores() {
  digitalWrite(Motorrojo, LOW);
  delayMicroseconds(2);
  digitalWrite(MotorNaranja, LOW);
  delayMicroseconds(2);
  digitalWrite(Motoramarillo, LOW);
  delayMicroseconds(2);
  digitalWrite(Motorverde, LOW);
}
void moverMotores() {
  digitalWrite(Motorrojo, HIGH);
  delayMicroseconds(2);
  digitalWrite(MotorNaranja, LOW);
  delayMicroseconds(2);
  digitalWrite(Motoramarillo, LOW);
  delayMicroseconds(2);
  digitalWrite(Motorverde, HIGH);
}
void avanzarMotores() {
  digitalWrite(Motorrojo, LOW);
  delayMicroseconds(2);
  digitalWrite(MotorNaranja, HIGH);
  delayMicroseconds(2);
  digitalWrite(Motoramarillo, LOW);
  delayMicroseconds(2);
  digitalWrite(Motorverde, HIGH);
}

//--------------------------------------TAREAS
//------------------------------------------Tarea 1 Motores y ultrasónico. Si se acaba el tiempo se detiene hasta reiniciar. Si se atora, se detiene hasta reacomodarlo.
void TareaMotores(void *parameter) {
while(1){  
digitalWrite(Trigger, LOW);     //Medición del ultrasónico
delayMicroseconds(2);  
digitalWrite(Trigger, HIGH);
delayMicroseconds(10);          //Pulso de 10us
digitalWrite(Trigger, LOW);
t = pulseIn(Echo, HIGH); //Ancho del pulso
d = t*0.034/2;        //Medición en cm.     
delay(100);          
//Lógica pa que no choque
  if(tiempoExpirado){
      detenerMotores();
  }else{
  if (d <= 31) {
        if (!detenerPorTiempoExcedido) {
          if (millis() - tiempoIntentoInicio >= tiempoIntentoMaximo) {
            detenerPorTiempoExcedido = true;
            detenerMotores();
          }
        }
        if (!detenerPorTiempoExcedido) {
          moverMotores();
          delay(random(500, 1000));
        }
      } else{
          tiempoIntentoInicio = millis();
          detenerPorTiempoExcedido = false;
              avanzarMotores();         
   }
  }
 }
}
//---------------------------------------------------------------Tarea 2 PWM que simula recoger basura y parpadea cuando se acaba el tiempo de funcionamiento. Si el carro se atora, PWM=0.
void TareaPWM(void *parameter) {
const TickType_t xFrequency = 10;
int pwmvalor = 0;
iniciarTemporizador();
while(1){  
  verificarTiempo();  // Verificar si el tiempo ha expirado, si sí, deja de moverse y parpadea el led
if (tiempoExpirado){
  while (tiempoExpirado) {
    analogWrite(ledpwm, 255);
    delay(1000);
    analogWrite(ledpwm, 0);
    delay(1000);
  }
}else{
  analogWrite(ledpwm, pwmvalor);
  pwmvalor += 1;
  if (pwmvalor > 255){
      pwmvalor = 0;}
  if (detenerPorTiempoExcedido) {//Si pasa demasiado tiempo atorado, el PWM tiene valor de 0.
    analogWrite(ledpwm, 0);
    delay(50);
    }
  vTaskDelay(pdMS_TO_TICKS(xFrequency));
   }
  }
 }
//---------------------------------------------------------------Tarea de comunicación serial bluetooth. Comunica el nivel de la batería, que es un potenciómetro.
 void TareaBT(void *parameter) {
 while(1){  
   int Potenciometro = analogRead(36);
   int bateria = map(Potenciometro, 0, 4095, 0, 100);// Mapea el valor a un rango de 0 a 100
   SerialBT.print("bateria: ");
   SerialBT.println(bateria);
   vTaskDelay(pdMS_TO_TICKS(1000));
   }
}
//---------------------------------------------------------------Tarea de alerta si se atora. Enciende un led si está atorado, se apaga si no lo está.
 void AlertaPAPU(void *parameter) {
 while(1){  
  if (detenerPorTiempoExcedido) {
    digitalWrite(ledalerta, HIGH);
    delay(50);
    }else{
    digitalWrite(ledalerta, LOW);
    delay(50);
    }
 }
}

void setup(){
pinMode(Motorrojo, OUTPUT); //Pin de motor izquierdo
pinMode(MotorNaranja, OUTPUT); //Pin de motor izquierdo
pinMode(Motoramarillo, OUTPUT); //Pin de motor derecho
pinMode(Motorverde, OUTPUT); //Pin de motor derecho
pinMode(ledpwm, OUTPUT); //Led del PWM
pinMode(ledalerta, OUTPUT); //Led de alerta
pinMode(Trigger, OUTPUT); //pin trigger del ultrasónico
pinMode(Echo, INPUT);  //pin echo del ultrasónico
digitalWrite(Trigger, LOW);//Trigger inicializado en 0.
SerialBT.begin("ESP32_BTSerial"); //Nombre bluetooth del ESP.

Serial.begin(9600);
  xTaskCreatePinnedToCore( //Tarea motores, nucleo 1
              TareaMotores, 
              "TareaMotores", 
              1024,
              NULL,
              1,
              &Motores,
              1);
  xTaskCreatePinnedToCore( //Tarea PWM, nucleo 1
              TareaPWM,
              "TareaPWM", 
              1024,
              NULL,
              1,
              &PWM,
              1);
  xTaskCreatePinnedToCore( //Tarea bluetooth, nucleo 0
               TareaBT, 
               "TareaBT", 
               1024,
               NULL,
               1,
               &BT,
               0);
xTaskCreatePinnedToCore( //Tarea de alerta, nucleo 0
               AlertaPAPU, 
               "AlertaPAPU",
               1024,
               NULL,
               1,
               &Alerta,
               0);
}
void loop(){
}