/*
  Arduino Wireless Network - Multiple NRF24L01 Tutorial
        == Node 02 (Child of Master node 00) ==    
*/
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <MaxMatrix.h>
#include <SimpleDHT.h>

RF24 radio(49, 48);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 01;   // Address of our node in Octal format ( 04,031, etc)
const uint16_t master00 = 00;    // Address of the other node in Octal format
const unsigned long interval = 1000;  //ms  // How often to send data to the other unit
unsigned long last_sent;            // When did we last send?

int ledPin = 13;                // choose the pin for the LED
int inputPin = 4;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
 
int DIN = 12;   // DIN pin of MAX7219 module
int CLK = 10;   // CLK pin of MAX7219 module
int CS = 11;    // CS pin of MAX7219 module
int maxInUse = 1;

int pinDHT11 = 2;
SimpleDHT11 dht11(pinDHT11);


MaxMatrix m(DIN, CS, CLK, maxInUse); 

byte spriteX[] = {8,8,B10000001,B01000010,B00100100,B00011000,B00011000,B00100100,B01000010,B10000001};
byte spriteRED[] = {8,8, B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111};
byte Array[3];
void setup() {

  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
  Serial.begin(9600);
  Serial.print("#Hi Mano#");
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  
  m.init(); // MAX7219 initialization
  m.setIntensity(8); // initial led matrix intensity, 0-15
}
void loop() {
  network.update();
  //===== Sending =====//
  unsigned long now = millis();
  if (now - last_sent >= interval) {   // If it's time to send a data, send it!
    last_sent = now;
m.clear();
  network.update();

      // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
    return;
  }

  Serial.print((int)temperature); Serial.print(" *C, "); 
  Serial.print((int)humidity); Serial.println(" H");

    
    unsigned long potValue = analogRead(A1);
    unsigned long ledBrightness = map(potValue, 0, 1023, 0, 255);
     Serial.println(ledBrightness);
Array[0]=(int)temperature;
Array[1]=(int)humidity;
Array[2]=ledBrightness;
Array[3]=0;

      network.update();

    RF24NetworkHeader header(master00);   // (Address where the data is going)
    bool ok = network.write(header, &Array, sizeof(Array)); // Send the data
      network.update();

  }
  
  //===== Receiving =====//
  while ( network.available() ) {     // Is there any incoming data?
    m.clear();
      network.update();

    RF24NetworkHeader header;
    unsigned long incomingData;
    network.read(header, &incomingData, sizeof(incomingData)); // Read the incoming data
    if (header.from_node == 0) {    // If data comes from Node 02
      Serial.println(incomingData);  // tell servo to go to a particular angle
      if (incomingData == 0){
          network.update();

        Serial.println("Alert!");
        for (int xx=1; xx < 3; xx++){
          m.writeSprite(0, 0, spriteRED);
           delay(1000);
    network.update();

          m.clear();
          delay(1000);
    network.update();

          m.writeSprite(0, 0, spriteX);
          delay(1000);
            network.update();

        }
      }
      else 
        m.clear();
          network.update();

        //Serial.println("No");
    }
      network.update();

    if (header.from_node == 10) {    // If data comes from Node 012
      digitalWrite(ledPin, !incomingData);  // Turn on or off the LED 02
    }
}
}
