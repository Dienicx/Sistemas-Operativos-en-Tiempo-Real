static int no_tarea = 1000;

SemaphoreHandle_t barridoSemaforo = xSemaphoreCreateCounting(no_tarea,0);

void tareaGenerica (void * parameter){
  String mns_generido = "Tarea #: ";
  Serial.print(mns_generido);
  Serial.println((int)parameter); 
  xSemaphoreGive(barridoSemaforo);
  vTaskDelete(NULL);
}

void setup(){
  Serial.begin(115200);
  Serial.println("Inicio del programa");
  for(int i =0;i<no_tarea;i++){
    xTaskCreatePinnedToCore(tareaGenerica,
                            "Tarea Generica",
                            1024,
                            (void *)i,
                            0,        
                            NULL,
                            1);
  vTaskDelay(2 / portTICK_PERIOD_MS);                        
  }
  for(int i =0;i<no_tarea;i++){
    xSemaphoreTake(barridoSemaforo, portMAX_DELAY);
  }
  Serial.println("Tareas cargadas en semaforo");
}

void loop(){
  vTaskSuspend(NULL);
  //vTaskDelay(10000 / portTICK_PERIOD_MS);
}