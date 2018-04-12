#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = {'s','n','o','o','p','y'};
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}
void loop() {
  if (radio.available()) {
    uint8_t buf[40];
    radio.read(&buf, 40);
    Serial.print("0:");
    Serial.println(buf[0]);
    Serial.print("1:");
    Serial.println(buf[1]);
    
    //ADC 2 bytes.. little endian encoding
    Serial.print("2:");
    uint16_t sensor0adc = buf[2];
    sensor0adc |= (buf[3] << 8);
    Serial.println(sensor0adc);
    
    Serial.print("3:");
    uint16_t sensor1adc = buf[4];
    sensor1adc |= (buf[5] << 8);
    Serial.println(sensor1adc);
    
    //State
    Serial.print("4:");
    Serial.println(buf[6]);
    
    //String
    Serial.print("5:");
    for(uint8_t i = 0; i < buf[7]; i++){
      Serial.print((char)buf[8+i]);
    }
    Serial.println();
  }
}
