#include <Conceptinetics.h> 

#define DMX_MASTER_CHANNELS 100 

#define RXEN_PIN 2 

DMX_Master dmx_master ( DMX_MASTER_CHANNELS, RXEN_PIN ); 

void setup() { 
  dmx_master.enable (); 
  
  Serial.begin(9600);
  Serial.println("SerialToDmx ready");
  Serial.println();
  Serial.println("Syntax:");
  Serial.println(" 123c : use DMX channel 123");
  Serial.println(" 45w  : set current channel to value 45");
} 

int value = 0;
int channel;

void loop() {  
  int c;

  while(!Serial.available());
  c = Serial.read();
  if ((c>='0') && (c<='9')) {
    value = 10*value + c - '0';
  } else {
    if (c=='c') channel = value;
    else if (c=='w') {
      dmx_master.setChannelValue (channel, value ); 
      Serial.println();
    }
    value = 0;
  }
} 
