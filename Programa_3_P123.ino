#if CONFIG_FREERTOS_UNICROE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif
const char msg1[]="Tarea 1 corriendo";
const char msg2[]="Tarea 2 corriendo";
const char msg3[]="Tarea 3 corriendo";
static TaskHandle_t Tarea01 = NULL;
static TaskHandle_t Tarea02 = NULL;
static TaskHandle_t Tarea03 = NULL;
//tarea 1
void Tarea1(void *parameter) {
int msg1_len = strlen(msg1);
while(1){
  Serial.println();
  for(int i=0;i<msg1_len;i++){
    Serial.print(msg1[i]);
  }
  Serial.println();
  vTaskDelay(1000 / portTICK_PERIOD_MS);
 }
}

//TAREA 2
void Tarea2(void *parameter) {
int msg2_len = strlen(msg2);
while(1){
  Serial.print('-');
  for(int i=0;i<msg2_len;i++){
    Serial.print(msg2[i]);
  }
  Serial.println();
  vTaskDelay(1000 / portTICK_PERIOD_MS);
 }
}

//TAREA 3
void Tarea3(void *parameter) {
int msg3_len = strlen(msg3);
while(1){
  Serial.print('-');
  for(int i=0;i<msg3_len;i++){
    Serial.print(msg3[i]);
  }
  Serial.println();
  vTaskDelay(1000 / portTICK_PERIOD_MS);
 }
}
void setup(){
Serial.begin(300);
  xTaskCreatePinnedToCore(
              Tarea1, //funcion llamda
              "Tarea1", //nombre
              1024,
              NULL,
              2,
              &Tarea01,
              app_cpu);
  Serial.begin(300);
  xTaskCreatePinnedToCore(
              Tarea2, //funcion llamda
              "Tarea2", //nombre
              1024,
              NULL,
              1,
              &Tarea02,
              app_cpu);
    Serial.begin(300);
  xTaskCreatePinnedToCore(
              Tarea3, //funcion llamda
              "Tarea3", //nombre
              1024,
              NULL,
              7,
              &Tarea03,
              app_cpu);
}

void loop(){
  while(1){
    vTaskDelete(Tarea01);
    vTaskSuspend(Tarea02);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    vTaskResume(Tarea02);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}