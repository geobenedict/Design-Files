/* 

The code below is used for getting the GPS loaction using the GPS module NEO-M8N
and transmitting the data through the LORA-RF95M transceiver. Also for data logging purpose,
data is been send to the SD card present with in the PCB board.
****NB: The GPS data needs to be logged for tracking

-----------------------------------------------------------------------------------------------
*/


#include <TinyGPS++.h>      //include 
#include <SPI.h>
#include <LoRa.h>
#include <SD.h>

// GPS module settings
#define GPS_SERIAL Serial2  // PIN where GPS module is connected 
#define GPS_BAUDRATE 9600  // Set the Baud rate which is compatible with the GPS module

// LoRa settings
#define LORA_CS_PIN &&   // Provide the LoRa chip select pin number
#define LORA_RST_PIN 6  // LoRa reset pin
#define LORA_INT_PIN 14  // LoRa DIO0 pin

// SD card settings
// If an SD card/memory card is used for storing the GPS data
#define SD_CS_PIN &&  // Provide the CS pin for SD card module

// Create a TinyGPS++ object
TinyGPSPlus gps; // An object named 'gps' is created to interact with the TinyGPS plus library.
                //This object receives useful information from the raw data strings transmitted by the GPS mosule 

// File object for SD card
File dataFile;

void setup() {
  Serial.begin(115200);  // For debugging
  GPS_SERIAL.begin(GPS_BAUDRATE);  // Initialize GPS serial communication

  // Initialize LoRa module
  LoRa.setPins(LORA_CS_PIN, LORA_RST_PIN, LORA_INT_PIN);
  if (!LoRa.begin(915E6)) //915MHZ frequency for operation
  {
    Serial.println("LoRa initialization failed. Check your connections.");
    while (1);
  }

  // Initialize SD card
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD card initialization failed. Check your connections.");
    while (1);
  }

  Serial.println("Initialization completed");
}

void loop() {
  // Read data from GPS module
  while (GPS_SERIAL.available() > 0) {
    if (gps.encode(GPS_SERIAL.read())) {
      
      // Send GPS data over LoRa
      sendGPSDataOverLoRa();

      // Log GPS data to SD card
      logGPSDataToSD();
    }
  }
}


void sendGPSDataOverLoRa() {
  // Check if GPS data is valid
  if (gps.location.isValid() && gps.date.isValid() && gps.time.isValid()) {
    // Construct a message containing GPS data
    String message = String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6) + "," +
                     String(gps.date.month()) + "/" + String(gps.date.day()) + "/" + String(gps.date.year()) + " " +
                     String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second());

    // Send message over LoRa
    LoRa.beginPacket();
    LoRa.print(message);
    LoRa.endPacket();
  }
}

void logGPSDataToSD() {
  // Open file on SD card in append mode
  dataFile = SD.open("gps_data.txt", FILE_WRITE);
  
  // If the file is available, write GPS data to it
  if (dataFile) {
    if (gps.location.isValid() && gps.date.isValid() && gps.time.isValid()) {
      dataFile.print("Location: ");
      dataFile.print(gps.location.lat(), 6); // lattitude formatted to 6 decimal places 
      dataFile.print(",");
      dataFile.print(gps.location.lng(), 6);
      dataFile.print("  Date/Time: ");
      dataFile.print(gps.date.month());
      dataFile.print("/");
      dataFile.print(gps.date.day());
      dataFile.print("/");
      dataFile.print(gps.date.year());
      dataFile.print(" ");
      dataFile.print(gps.time.hour());
      dataFile.print(":");
      dataFile.print(gps.time.minute());
      dataFile.print(":");
      dataFile.print(gps.time.second());
      dataFile.println();
    } else {
      dataFile.println("GPS data invalid");
    }
    
    dataFile.close();  // Close the file
  } else {
    Serial.println("Error opening file for writing");
  }
}

