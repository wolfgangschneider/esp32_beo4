#include <Arduino.h>
#include "esp_log.h"  // Add this line for ESP_LOGI
#include "IrBeo4.h"


#if defined(m5stackAtomS3)
constexpr int8_t con_IR_RX_PIN = 8;  // IR receive pin
#elif defined(m5stackAtom)
constexpr int8_t con_IR_RX_PIN = 33; // IR receive pin
#else
constexpr int8_t con_IR_RX_PIN = 15;  // IR receive pin
#endif
IrBeo4 beo4(con_IR_RX_PIN);           // instance of beo4 decoder

// solution with task and queue
// 
xQueueHandle beo4_rx_queue;     // queue 
TaskHandle_t beo4_task_h;       // task handle

void beo4_task(void *parameter) {
  static uint32_t beo4Code=0;
  while(1) {
    if(pdTRUE==xQueueReceive(beo4_rx_queue,(void*)(&beo4Code),portMAX_DELAY)) {
      Serial.printf("beo4_task:   %04x %s %s\n",beo4Code,beo_src_tbl(beo4Code),beo_cmd_tbl(beo4Code));
    }
  }
}

// alternative solution with callback function
//
// void beo_code_cb(uint32_t beo_code) {
//   Serial.printf("beo_code_cb: %04x %s %s \n", beo_code,beo_src_tbl(beo_code),beo_cmd_tbl(beo_code));
// }

// led call back function
//
// constexpr uint8_t led_pin=2;
// void beo_led_cb(uint8_t mode){
//   digitalWrite(led_pin,mode);
// }

void setup() {
  Serial.begin(115200);
 
  // Add these test lines
  // Set log level for your library
  //esp_log_level_set("IR_BEO4", ESP_LOG_INFO);


  
  
  
  
  beo4_rx_queue = xQueueCreate(50, sizeof(uint32_t));
  xTaskCreatePinnedToCore(beo4_task,"beo4_task",10000,NULL,0,&beo4_task_h,0);
  beo4.Begin(beo4_rx_queue);
  printf("beo4 started, RX=%d\n",con_IR_RX_PIN);
}

void loop() {
}

