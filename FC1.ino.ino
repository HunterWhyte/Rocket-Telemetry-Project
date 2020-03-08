/* Receiver code for the Arduino Radio control with PWM output
 * Install the NRF24 library to your IDE
 * Upload this code to the Arduino UNO, NANO, Pro mini (5V,16MHz)
 * Connect a NRF24 module to it:
 
    Module // Arduino UNO,NANO
    
    GND    ->   GND
    Vcc    ->   3.3V
    CE     ->   D9
    CSN    ->   D10
    CLK    ->   D13
    MOSI   ->   D11
    MISO   ->   D12

This code receive 1 channels and prints the value on the serial monitor
Please, like share and subscribe : https://www.youtube.com/c/ELECTRONOOBS
*/


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

int i = 0;

const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };    //Remember that this code is the same as in the transmitter
RF24 radio(9, 10);  //CSN and CE pins

// The sizeof this struct should not exceed 32 bytes
struct Received_data {
  byte ch1;
  byte ch2;
};

int ch1_value = 0;
int ch2_value = 0;
Received_data received_data;

struct Data_to_be_sent {
  byte height; 
  byte a; 
};
Data_to_be_sent sent_data;


/**************************************************/

void setup()
{
  Serial.begin(9600);
  //We reset the received values
  received_data.ch1 = 0;
  received_data.ch2 = 0;
  sent_data.height = 0;
  sent_data.a = 0;
  //Once again, begin and radio configuration
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);  
  radio.openWritingPipe(pipes[1]);  
  radio.openReadingPipe(1,pipes[0]);
  
  //We start the radio comunication
  radio.startListening();

}

/**************************************************/

unsigned long last_Time = 0;

//We create the function that will read the data each certain time
void receive_the_data()
{
  Serial.println("recieving");
  while ( radio.available() ) {
  radio.read(&received_data, sizeof(Received_data));
  last_Time = millis(); //Here we receive the data
}
}


void send_the_data()
{
  Serial.println("sending");
  sent_data.height = 255;
  sent_data.a = 255;
  radio.write(&sent_data, sizeof(Data_to_be_sent));
}

/**************************************************/

void loop()
{
  /*radio.startListening();
  
    //Receive the radio data
    receive_the_data();
    Serial.println("recieving");
  
    ch1_value = map(received_data.ch1,0,255,1000,2000);
    ch2_value = map(received_data.ch2,0,255,1000,2000);
    Serial.println(ch1_value);
    Serial.println(ch2_value);*/
  
  radio.stopListening();

    send_the_data();
  

}//Loop end

