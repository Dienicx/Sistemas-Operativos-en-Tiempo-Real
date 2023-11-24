#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

/*Variable global*/
static int buf[5];                        //Buffer
static int apuntador = 0;                 //Index consumidor
static SemaphoreHandle_t bin_sem;

static void Productor(void *parameter){
  int num = 32;
  Serial.println("Tarea del productor");
  while(1){
    xSemaphoreGive(bin_sem);
    vTaskDelay(200 / portTICK_PERIOD_MS);
    buf[apuntador] = num;
    Serial.println("Registro listo");
  }
}
static void Consumidor(void *parameter){
  int valor;
  while(1){
    if(xSemaphoreTake(bin_sem, portMAX_DELAY) == true){
    Serial.println("Consumidor");
    valor = buf[apuntador];
    Serial.println(valor);
    }
  }
}

void setup() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.begin(115200);
  Serial.println("--Mensaje de inicio--");

  bin_sem = xSemaphoreCreateBinary();

  xTaskCreatePinnedToCore(Productor,"Productor",1024,NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(Consumidor,"Consumidor",1024,NULL,1,NULL,app_cpu);

}

void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
