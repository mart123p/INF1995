//
// Sensor packet should be the following form
//
// 0x11   sensor0     sensor1
//

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const byte address[6] = {'s','n','o','o','p','y'};

uint8_t sensor0 = 0;
uint8_t sensor1 = 0;
uint8_t sensor0adc[2];
uint8_t sensor1adc[2];

uint8_t state = 0;
bool dataReady = false;
uint8_t strBytes[32];
uint8_t byteString = 0;

void setup() {
  Serial.begin(2400);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.stopListening();
}

void loop() {
  if(dataReady){
    dataReady = false;

    uint8_t dataToTransmit[40];
    dataToTransmit[0] = sensor0;
    dataToTransmit[1] = sensor1;
    
    dataToTransmit[2] = sensor0adc[0];
    dataToTransmit[3] = sensor0adc[1];
    dataToTransmit[4] = sensor1adc[0];
    dataToTransmit[5] = sensor1adc[1];
    
    dataToTransmit[6] = state;
    dataToTransmit[7] = byteString;
    for(uint8_t i = 0; i < byteString; i++){
      dataToTransmit[8+i] = strBytes[i];
    }

    radio.write(&dataToTransmit,40);
    
  }

}

void serialEvent() {
  while (Serial.available()) {
    if(Serial.available() >= 9){
      uint8_t headerByte = Serial.read();
      if(headerByte == 0x11){
        //We read the next three bytes
        sensor0 = Serial.read();
        sensor1 = Serial.read();
        
        sensor0adc[0] = Serial.read();
        sensor0adc[1] = Serial.read();

        sensor1adc[0] = Serial.read();
        sensor1adc[1] = Serial.read();
        
        state = Serial.read();
        byteString = Serial.read();
        
        for(uint8_t i = 0; i < byteString; i++){
          while(!Serial.available()); //Wait for the next data to arrive
          if(i >= 32){
            byteString = 32;
            break;
          }
          strBytes[i] = Serial.read();
        }
        dataReady = true;
      }
    }
  }
}
