/*
The code below is used to set the ESP32 microcontroller to light sleep mode

------------------------------------------------------------------------------
*/


void setup() {
  Serial.begin(115200);  // put your setup code here, to run once:

}

void loop() { // put your main code here, to run repeatedly:
  Serial.println("light_sleep_enter");
  esp_sleep_enable_timer_wakeup(1000000); //1 seconds
  int ret = esp_light_sleep_start();
  Serial.printf("light_sleep: %d\n", ret);
 

}
