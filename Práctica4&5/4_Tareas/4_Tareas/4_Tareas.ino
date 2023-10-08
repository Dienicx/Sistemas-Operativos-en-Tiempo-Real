boolean do_once = false;
//Task and Queue handlers
TaskHandle_t Task1; 
TaskHandle_t Task2; 
TaskHandle_t Task3; 
TaskHandle_t Task4; 

static void Tarea1( void * parameter);
static void Tarea2( void * parameter);
static void Tarea3( void * parameter);
static void Tarea4( void * parameter);

bool trn1 = false;
bool trn2 = false;
bool trn3 = false;
bool trn4 = false;
int turno = 0;

char mensaje = ' ';

static void Tarea1( void * parameter) {
  while(1){
    trn1 = true;
    turno=1;
    while(trn2 && trn3 && trn4 && turno==1){
      //T0 Esperando
    }
    trn1 = false;
    vTaskDelay(500 / portTICK_PERIOD_MS); //wait for a second
  }
}
static void Tarea2( void * parameter) {
  while(1){
    trn2 = true;
    turno=2;
    while(trn1 && trn3 && trn4 && turno==2){
      //T1 Esperando
    }
    trn2 = false;
    vTaskDelay(500 / portTICK_PERIOD_MS); //wait for a second
  }
}
static void Tarea3( void * parameter) {
  while(1){
    trn3 = true;
    turno=3;
    while(trn1 && trn2 && trn4 && turno==3){
      //T0 Esperando
    }
    trn3 = false;
    vTaskDelay(500 / portTICK_PERIOD_MS); //wait for a second
  }
}
static void Tarea4( void * parameter) {
  while(1){
    trn4 = true;
    turno=4;
    while(trn1 && trn2 && trn3 && turno==4){
      //T1 Esperando
    }
    trn4 = false;
    vTaskDelay(500 / portTICK_PERIOD_MS); //wait for a second
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Running");
  vTaskDelay(1000 / portTICK_PERIOD_MS); //wait for a second

  xTaskCreatePinnedToCore(Tarea1,"Tarea01",2048,NULL,1,&Task1,NULL);
  xTaskCreatePinnedToCore(Tarea2,"Tarea02",2048,NULL,1,&Task2,NULL);  
  xTaskCreatePinnedToCore(Tarea3,"Tarea03",2048,NULL,1,&Task1,NULL);
  xTaskCreatePinnedToCore(Tarea4,"Tarea04",2048,NULL,1,&Task2,NULL);
}

void loop() {
  if (Serial.available() !=0){
        mensaje=Serial.read();
        if(mensaje=='a'){
          Serial.println("Se está ejecutando la tarea 1");
       }
    }
  else if (Serial.available() !=0){
        mensaje=Serial.read();
        if(mensaje=='b'){
          Serial.println("Se está ejecutando la tarea 2");
       }
    }
  else if (Serial.available() !=0){
        mensaje=Serial.read();
        if(mensaje=='c'){
          Serial.println("Se está ejecutando la tarea 3");
       }
    }
  else if (Serial.available() !=0){
        mensaje=Serial.read();
        if(mensaje=='d'){
          Serial.println("Se está ejecutando la tarea 4");
       }
    }
}
