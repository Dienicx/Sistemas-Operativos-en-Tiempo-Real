static const uint8_t msg_queue_longitud = 5;
//Fila global
static QueueHandle_t msg_queue;

void setup (){
  Serial.begin(115200);
  msg_queue = xQueueCreate(msg_queue_longitud,sizeof(int));
  for(int j=0; j<msg_queue_longitud; j++){
    if(j%2==0){
      xTaskCreatePinnedToCore(DejaPares,"Pares",1024,NULL,1,NULL,1);
      vTaskDelay(2 / portTICK_PERIOD_MS);
      xTaskCreatePinnedToCore(VaciaPares,"Pares",1024,NULL,1,NULL,1);
      vTaskDelay(2 / portTICK_PERIOD_MS);
    }else{
      xTaskCreatePinnedToCore(DejaImpares,"Impares",1024,NULL,1,NULL,1);
      vTaskDelay(2 / portTICK_PERIOD_MS);
      xTaskCreatePinnedToCore(VaciaImpares,"Impares",1024,NULL,1,NULL,1);
      vTaskDelay(2 / portTICK_PERIOD_MS);
    }
  }
}

void DejaPares (void *parameters){
  while(1){
    static int i = 0;
    if(xQueueSend(msg_queue,(void*)&i,5)!=true){
      Serial.println("Casilla Par llena");
    }else{
      String mns_Par = "Dejando tarea # ";
      Serial.print(mns_Par);
      Serial.println(i);
      vTaskDelay(1000/ portTICK_PERIOD_MS);
    }
    i++;
  }
}
void VaciaPares (void *parameters){
  int valorP;
  while(1){
    if(xQueueReceive(msg_queue,(void*)&valorP,0)== true){
      String mns_genericoP = "Recogiendo tarea # ";
      Serial.print(mns_genericoP);
      Serial.println(valorP);
      vTaskDelay(1000/ portTICK_PERIOD_MS);
    }
  }
}
void DejaImpares (void *parameters){
  while(1){
    static int i = 0;
    if(xQueueSend(msg_queue,(void*)&i,5)!=true){
      Serial.println("Casilla Par llena");
    }else{
      String mns_Impar = "Dejando tarea # ";
      Serial.print(mns_Impar);
      Serial.println(i);
      vTaskDelay(1000/ portTICK_PERIOD_MS);
    }
    i++;
  }
}
void VaciaImpares (void *parameters){
  int valorI;
  while(1){
    if(xQueueReceive(msg_queue,(void*)&valorI,0)== true){
      String mns_genericoI = "Recogiendo tarea # ";
      Serial.print(mns_genericoI);
      Serial.println(valorI);
      vTaskDelay(1000/ portTICK_PERIOD_MS);
    }
  }
}

void loop(){

}