/*
ESp32-C3 module reads the sensor data coming through the RS232 
switch into the UART port serial interface through the DB9 female port
---------------------------------------------------------------------------------
*/

//define ESP32 UART PINS ( Used ESP32-C3-M1 KIt)
#define ESP32_RX_PIN 27  // GPIO pin numebr for ESP32 RX
#define ESP32_TX_PIN 28  //GPIO pin number for ESP32 TX


//Define DB9 pinout for RS232 communication
#define ESP32_RX_PIN 3  // pin number on DB9 connector for receiving the data
#define ESP32_TX_PIN 2  // pin number on DB9 connector for transmitting the data

//Define RS232 port settings for receiving data from RS232 into the DB9 port
#define RS232_SERIAL Serial1
#define RS232_BAUDRATE 9600  //Give Baud rate that is supported by the RS232 device.

//LoRa settings for tranmsitting the data
#define LORA_CS_PIN &&   // Give the corresponding pin number for LOra chip select pin
#define LORA__RST_IN &&  //Give the corresponding pin number for LOra chip reset pin
#define LORA_INT_PIN &&  //Give the corresponding pin number for LOra Digital input pin

void setup() {
  Serial.begin(9600);  // select the baud rate which is compatible with RS232

  //Initialize the LORA module
  LoRa.setPins(LORA_CS_PIN, LORA_RST_PIN, LORA_INT_PIN);

  if (!LoRa.begin(915E6))  // 915MHz is the specified frequency range of the LORA module & the begin function returns a  boolean value
  {
    Serial.println("LoRa initialization failed. Please check your connections.");
    while (1)
      ;
  }
  Serial.println("LoRa initialized");
}

void loop() {
  // Read the data from the RS232 port and if the data is received, then
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');  // received data is stored in a string variable called 'data'

    // Send data over LoRa
    LoRa.beginPacket();  // Starts the tranmsision of data over the LoRa network and sends the data
    LoRa.print(data);    // Sends the data stored in the 'data' variable over the LoRa network
    LoRa.endPacket();    // Completes the transmission process

    Serial.println("Data sent over LoRa: " + data);  // Only use if a monitor is attached to check the data that's been send or else avoid this line
  }
}
}
