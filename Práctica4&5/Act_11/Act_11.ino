boolean do_once = false;
//Task and Queue handlers
TaskHandle_t Task1; 
TaskHandle_t Task2; 

static void Tarea1( void * parameter);
static void Tarea2( void * parameter);

bool trn1 = false;
bool trn2 = false;
int turno = 0;

char mensaje = ' ';

static void Tarea1( void * parameter) {
  while(1){
    trn1 = true;
    turno=1;
    while(trn2 && turno==1){
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
    while(trn1 && turno==2){
      //T1 Esperando
    }
    trn2 = false;
    vTaskDelay(1000 / portTICK_PERIOD_MS); //wait for a second
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Running");
  vTaskDelay(1000 / portTICK_PERIOD_MS); //wait for a second

  xTaskCreatePinnedToCore(Tarea1,"Tarea01",2048,NULL,1,&Task1,NULL);
  xTaskCreatePinnedToCore(Tarea2,"Tarea02",2048,NULL,1,&Task2,NULL);  
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
}
