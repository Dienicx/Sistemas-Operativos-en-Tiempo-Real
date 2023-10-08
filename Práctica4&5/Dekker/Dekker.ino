//Utilizamos sólo core 1
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu=0;
#else
static const BaseType_t app_cpu=1;
#endif

boolean wp1=false;
boolean wp2=false;
int turno = 1;

//Tarea 01
void Proceso1(void *parameter){
  while(true){
    wp1=true;
    while(wp2){
      if(turno==2){
        wp1=false;
        while(turno==2){}
        wp1=true;
      }
    }
    turno=2;
    wp1=false;
    Serial.println("Proceso 1 ejecutándose");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

//Tarea UART 02
void Proceso2(void *parameter){
  while(true){
    wp2=true;
    while(wp1){
      if(turno==2){
        wp2=false;
        while(turno==2){}
        wp2=true;
      }
    }
    turno=2;
    wp2=false;
    Serial.println("Proceso 2 ejecutándose");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  xTaskCreatePinnedToCore(
    Proceso1,  //Función que se manda llamar
    "Proceso 01",  //Nombre de la tarea
    1024,         //Stack size
    NULL,         //
    1,            //Prioridades
    NULL,         //
    app_cpu);     //Core
    xTaskCreatePinnedToCore(
    Proceso2,  //Función que se manda llamar
    "Proceso 02",  //Nombre de la tarea
    1024,         //Stack size
    NULL,         //
    1,            //Prioridades
    NULL,         //
    app_cpu);     //Core
}

void loop() {
}
