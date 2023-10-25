# Bluepill-Arduino

## Purpose

The STM32 code is designed to communicate with an Arduino via USART (Universal Synchronous Asynchronous Receiver Transmitter) to receive RFID card data and process it. It utilizes an LCD for display and two LEDs (Green and Red) to indicate whether the received RFID data matches an authorized ID.

## Project flow

1. **Initialization**:
   - The STM32 microcontroller initializes the clock settings and necessary peripherals.
   - USART (USART1) is configured for serial communication with the Arduino.
   - An LCD is initialized for displaying messages.
   - Two LEDs (Green and Red) are configured as output pins for indicating RFID card authorization.

2. **Main Loop**:
   - The STM32 microcontroller continuously waits for data from the Arduino.
   - When RFID data is received from the Arduino via USART, it is displayed on the LCD.
   - The received RFID data is compared with a predefined authorized ID.
   - Based on the comparison result:
     - If the received data matches the authorized ID, the Green LED is turned on, and the Red LED is turned off.
     - If the received data does not match the authorized ID, the Red LED is turned on, and the Green LED is turned off.

3. **Arduino (MFRC522) Code**:
   - The Arduino code initializes communication with the MFRC522 RFID module.
   - In the `loop` function, it checks for the presence of an RFID card.
   - If a card is detected, it reads the card's Unique Identifier (UID) and sends it to the STM32 via Serial communication (UART).
   - After sending the UID, the Arduino code goes back to scanning for cards.

4. **Communication between STM32 and Arduino**:
   - The Arduino reads RFID card data from the MFRC522 module.
   - When a card is detected, the Arduino converts the UID to a hexadecimal string and sends it to the STM32 microcontroller over the Serial interface (UART).
   - The STM32 receives the data, processes it, and controls the LEDs to indicate whether the RFID card is authorized or not.

5. **User Interaction**:
   - Users can place an RFID card on the MFRC522 module connected to the Arduino.
   - The system, through the STM32 and LEDs, provides immediate feedback to the user by illuminating either the Green or Red LED based on the card's authorization status.


## Hardware demo


https://github.com/alaawahba13/Bluepill-Arduino/assets/101985923/e3803855-33b0-413a-870b-6e2015ed10fd


## Simulation demo 
  - [Simulation](https://github.com/alaawahba13/Bluepill-Arduino/tree/main/Simulation)
    
## Circuit Diagram 

## Documentation 

## Authors

This code was developed by 
- [@Alaa Wahba](https://github.com/alaawahba13)
- [@Ahmed Atef](https://github.com/AhmedAtef283)
- [@Mohamed Abdullhaleam](https://github.com/Mohamedabdullhaleam)
- [@Gehad Alaa ](https://github.com/Gehad799)
- Hager Tarek 
- Hazem Abdelnaser
- Moaaz Ahmed

For any questions or issues, please contact the authors.
