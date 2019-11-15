/*
  Arduino Wireless Network - Multiple NRF24L01 Tutorial
          == Base/ Master Node 00==
  by Dejan, www.HowToMechatronics.com
  Libraries:
  nRF24/RF24, https://github.com/nRF24/RF24
  nRF24/RF24Network, https://github.com/nRF24/RF24Network
*/
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#define button 2
#define led 13
RF24 radio(49, 48);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 00;   // Address of this node in Octal format ( 04,031, etc)
const uint16_t node01 = 01;      // Address of the other node in Octal format
const uint16_t node02 = 02;
const uint16_t node022 = 022;

byte Array[3];
char ch;
void setup() {
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  pinMode(button, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  Serial.begin(115200);
  Serial.print("#Hi from Base Mano#");
}
void loop() {
  network.update();
  //===== Receiving =====//
  while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header;
    unsigned long incomingData;
    network.read(header, &Array, sizeof(Array)); // Read the incoming data
    //Serial.println ((int)Array[0]);
    Serial.print("##");
    Serial.print(header.from_node);
    Serial.print(":");
    Serial.print((int)Array[0]);
    Serial.print(":");
    Serial.print((int)Array[1]);
    Serial.print(":");
    Serial.print((int)Array[2]);
    Serial.println("##");
      network.update();
  }
  ch=(char)Serial.read();
      if (ch == 'a')
    {
        network.update();
      //Serial.println("Its AAAAAA");
      //===== Sending =====//
  // Servo control at Node 01
  unsigned long angleValue1 = 0; // Suitable for servo control
  RF24NetworkHeader header2(node01);     // (Address where the data is going)
  bool ok = network.write(header2, &angleValue1, sizeof(angleValue1)); // Send the data
      network.update();

    }
      if (ch == 'b')
    {
        network.update();
      //Serial.println("Its BBBB");
      //===== Sending =====//
  // Servo control at Node 02
  unsigned long angleValue2 = 0; // Suitable for servo control
  RF24NetworkHeader header3(node02);     // (Address where the data is going)
  bool ok = network.write(header3, &angleValue2, sizeof(angleValue2)); // Send the data
    network.update();
    }
}
