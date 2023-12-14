#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

const int ROJO_01 = 19;
const int AMARILLO_01 = 18;
const int VERDE_01 = 5;

static bool marca01 = false;
static bool marca02 = false;
static bool marca03 = false;
static bool marca04 = false;
static bool marca05 = false; //Transición para el estado de parpadeo

//Configuración de los botones
const int SW_01 = 0;
const int SW_02 = 2;

unsigned long reposo = 0; // Tiempo de los botones
int tiempo1=1;
int tiempo2=1;
int tiempo3=1;
bool marca_final=false;

static bool marca1 = false;
static bool marca2 = false;
static bool marca3 = false;
static bool marca4 = false;
static bool marca5 = false;

// Prototipos de tareas
void estado01(void *parameters);
void estado02(void *parameters);
void estado03(void *parameters);
void estado04(void *parameters);

//Tareas del botón
void secuencia01(void *parameters);
void secuencia02(void *parameters);
void secuencia03(void *parameters);
void secuencia04(void *parameters);
void secuencia05(void *parameters);
void salida01(void *parameters);
void salida02(void *parameters);
void salida03(void *parameters);

//Semáforos para los leds
static SemaphoreHandle_t xSemaphore1,xSemaphore2,xSemaphore3,xSemaphore4;
//Semáforos para los botones
static SemaphoreHandle_t xSemaphore5,xSemaphore6,xSemaphore7,xSemaphore8,xSemaphore9;


void setup() {
  Serial.begin(115200);

  //Pines de los botones
  pinMode(SW_01, INPUT);
  pinMode(SW_02, INPUT);

  //Pines de los LEDs
  pinMode(ROJO_01, OUTPUT);
  pinMode(AMARILLO_01, OUTPUT);
  pinMode(VERDE_01, OUTPUT);

  xSemaphore1 = xSemaphoreCreateBinary();
  xSemaphore2 = xSemaphoreCreateBinary();
  xSemaphore3 = xSemaphoreCreateBinary();
  xSemaphore4 = xSemaphoreCreateBinary();
  xSemaphore5 = xSemaphoreCreateBinary();
  xSemaphore6 = xSemaphoreCreateBinary();
  xSemaphore7 = xSemaphoreCreateBinary();
  xSemaphore8 = xSemaphoreCreateBinary();
  xSemaphore9 = xSemaphoreCreateBinary();

  xTaskCreatePinnedToCore(estado01,"estado01",1024,NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(estado02,"estado02",1024,NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(estado03,"estado03",1024,NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(estado04,"estado04",1024,NULL,1,NULL,app_cpu);
  
  xTaskCreatePinnedToCore(salida01,"salida01",1024,NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(salida02,"salida02",1024,NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(salida03,"salida03",1024,NULL,1,NULL,app_cpu);
  
  xTaskCreatePinnedToCore(secuencia01,"estado01",1024,NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(secuencia02,"estado02",1024,NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(secuencia03,"estado03",1024,NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(secuencia04,"estado04",1024,NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(secuencia05,"estado04",1024,NULL,1,NULL,app_cpu);
  

  xSemaphoreGive(xSemaphore1);
  xSemaphoreGive(xSemaphore5);  
}

void loop() {

}

void estado01(void *parameters){
  while (1) {
      xSemaphoreTake(xSemaphore1, portMAX_DELAY);
      marca04=false;
      marca01=true;
      vTaskDelay(1500 / portTICK_PERIOD_MS);
      xSemaphoreGive(xSemaphore2);
  }
}

void estado02(void *parameters){
  while (1) {
      xSemaphoreTake(xSemaphore2, portMAX_DELAY);
      marca01=false;
      marca02=true;
      vTaskDelay(1500 / portTICK_PERIOD_MS);
      xSemaphoreGive(xSemaphore3);
  }
}

void estado03(void *parameters){
  while (1) {
      xSemaphoreTake(xSemaphore3, portMAX_DELAY);
      marca02=false;
      marca03=true;
      vTaskDelay(1500 / portTICK_PERIOD_MS);
      xSemaphoreGive(xSemaphore4);
  }
}

void estado04(void *parameters){
  while (1) {
      xSemaphoreTake(xSemaphore4, portMAX_DELAY);
      marca03=false;
      marca04=true;
      vTaskDelay(1500 / portTICK_PERIOD_MS);
      xSemaphoreGive(xSemaphore1);
  }
}

void salida01(void *parameters){
  while(1){
    if(marca_final==false){
      if(marca02 == true ){
        digitalWrite(ROJO_01, HIGH);
      }else{
        digitalWrite(ROJO_01, LOW);
      }
    }else{
      digitalWrite(ROJO_01, LOW);
    }
  }
}
void salida02(void *parameters){
  while(1){
    if(marca_final==false){
      if(marca01 == true){
        digitalWrite(AMARILLO_01, HIGH);
      }
      else{
        digitalWrite(AMARILLO_01, LOW);
      }
    }else{
      digitalWrite(AMARILLO_01, HIGH);
      vTaskDelay(500 / portTICK_PERIOD_MS);
      digitalWrite(AMARILLO_01, LOW);
      vTaskDelay(500 / portTICK_PERIOD_MS);
    }
  }
}
void salida03(void *parameters){
  while(1){
    if(marca_final==false){
      if(marca03 == true || marca04 == true){
        digitalWrite(VERDE_01, HIGH);
      }else{
        digitalWrite(VERDE_01, LOW);
      }
    }else{
      digitalWrite(VERDE_01, LOW);
    }
  }
}
//Secuencia de las interrupciones del botón
void transiciones(int casos){
  switch(casos){
    case 1:
      marca1=true; marca2=false; marca3=false; marca4=false; marca5=false;      
      tiempo1=0; tiempo2=0; tiempo3=0;
    break;

    case 2:
      marca1=false; marca2=true; marca3=false; marca4=false; marca5=false;
      tiempo1=1; tiempo2=0; tiempo3=0;
    break;

    case 3:
      marca1=false; marca2=false; marca3=true; marca4=false; marca5=false;
      tiempo1=0; tiempo3=0;
    break;

    case 4:
      marca1=false; marca2=false; marca3=false; marca4=true; marca5=false;
      tiempo1=0; tiempo2=0;
    break;

    case 5:
      marca1=false; marca2=false; marca3=false; marca4=false; marca5=true;
      tiempo1=0; tiempo2=0;
    break;
  }
}
//Secuencia antes de presionar el botón  
void secuencia01(void *parameters){
  while (1) {
    transiciones(1);
    xSemaphoreTake(xSemaphore5, portMAX_DELAY);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    while(digitalRead(SW_01)==LOW);
    reposo=millis();
    xSemaphoreGive(xSemaphore6);
  }
}
//Si presiona el primer botón espera 2 segundos. Si se cumplen, cambia de secuencia
//Si no pasan los dos segundos, regresa a la secuencia 1
void secuencia02(void *parameters){
  while (1) {
    transiciones(2);
    xSemaphoreTake(xSemaphore6, portMAX_DELAY);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    if(digitalRead(SW_01)==LOW){
      tiempo1=0;
      xSemaphoreGive(xSemaphore5);
    }
    while(tiempo1==1){
      if(digitalRead(SW_01)==HIGH){
        if((millis()-reposo)>2000){
          tiempo1=0;
          tiempo2=1;
         xSemaphoreGive(xSemaphore7);
        }
      }else{
        tiempo1=0;
        xSemaphoreGive(xSemaphore5);
      }
    } 
  }
}
//Si el primer botón sigue presionado y luego se presiona el segundo, cambia de secuencia
//Si se suelta el primer botón, regresa a la primera secuencia 
void secuencia03(void *parameters){
  while (1) {
    transiciones(3);
    xSemaphoreTake(xSemaphore7, portMAX_DELAY);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    while(tiempo2==1){
      if(digitalRead(SW_01)==HIGH){
        if(digitalRead(SW_02)==HIGH){
          xSemaphoreGive(xSemaphore8);
        }
      }
      else{
        xSemaphoreGive(xSemaphore5);
      }
    }
  }
}
//El primer botón sigue presionado y se suelta el segundo botón. Cambia de secuencia 
//Si se suelta el primer botón, regresa a la primera secuencia 
void secuencia04(void *parameters){
  while (1) {
    transiciones(4);
    xSemaphoreTake(xSemaphore8, portMAX_DELAY);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    tiempo3=1;
    while(tiempo3==1){
      if(digitalRead(SW_01)==HIGH){
        if(digitalRead(SW_02)==LOW){
          tiempo3=0;
          xSemaphoreGive(xSemaphore9);
        }
      }
      else{
        tiempo3=0;
        xSemaphoreGive(xSemaphore5);
      }
    }
  }
}
//Si vuelve a presionar el segundo botón, regresa a la secuencia 1 (interrupción)
//Si suelta el primer botón la secuencia se cumple y activa el parpadeo
void  secuencia05(void *parameters){
  while (1) {
    transiciones(5);
    xSemaphoreTake(xSemaphore9, portMAX_DELAY);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    while(marca5==true){
      if(digitalRead(SW_02)==HIGH){
        xSemaphoreGive(xSemaphore5);
      }
      if(digitalRead(SW_01)==LOW){ 
          if(marca_final==false){
            marca_final=true;
          }
          else{
            marca_final=false;
          }
          marca5=false;
          xSemaphoreGive(xSemaphore5);
      }
    }
  }
}
