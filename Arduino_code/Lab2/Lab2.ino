#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key;


String ID_str = "";

void setup() {
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
}

void loop() {

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
    
  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  for (int i = 0; i < rfid.uid.size ; i++) {
    if (rfid.uid.uidByte[i] < 0x10) {
      ID_str += "0";  // Add leading zero if the byte is less than 0x10
    }
    ID_str += String(rfid.uid.uidByte[i], HEX);
  }
  for (int i = 0; i <rfid.uid.size*2 ; i++) { // *2 since byte 0x01 is sent as '0' '1' 
    Serial.print(ID_str[i]);
  }
  ID_str = "";
  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();



}
