#include <EEPROM.h>

int relePins[10] = { 2, 3, 5, 6, 7, 8, 9, 10, 11, 12 };
int dmxBeginChannel = 10;

int myId = 1;

int strobeOutputValue = 0;
long previousMillis = 0;        // will store last time LED was updated
long interval = 1000;           // interval at which to blink (milliseconds)

int value = 0;
int channel;
void setup() {
  dmxBeginChannel = EEPROM.read(1);
  for(int i = 0; i <= 10; i++) {
    pinMode(i+2, OUTPUT);
  }
  //Tell to serial monitor what channels etc. you are using
  Serial.begin(115200);
  sendChannelDataToSerial();
}

int strobeValue;

void loop() {
  strobettaa();
   int c;

  if(Serial.available()) {
  c = Serial.read();
  if ((c>='0') && (c<='9')) {
    value = 10*value + c - '0';
  } else {
    if (c=='c') channel = value;
    else if (c=='w') {
      if(channel-dmxBeginChannel == 0) {
        for(int i = 0; i <= 8; i++) {
          analogWrite(relePins[i], value);
        }
      }
      if(channel-dmxBeginChannel == 1) {
        strobeValue = value;
      }
      else if(channel == 1000 + myId) {
        dmxBeginChannel = value;
        EEPROM.write(1, value);
        sendChannelDataToSerial();
      }
    }
    value = 0;
  }
  }
}
void strobettaa() {
  if(strobeValue < 200 && strobeValue > 20) {
    
    interval = map(strobeValue, 200, 20, 5, 200);
    
    unsigned long currentMillis = millis();
 
    if(currentMillis - previousMillis > interval) {
      previousMillis = currentMillis;   
      
      if(strobeOutputValue == 0) {
        analogWrite(relePins[9], 255);
        strobeOutputValue = 255;
      }
      else {
        analogWrite(relePins[9], 0);
        strobeOutputValue = 0;
      }
      
    }
  }
  else if(strobeValue < 20) {
    analogWrite(relePins[9], 0);
  }
  else if(strobeValue > 200) {
    analogWrite(relePins[9], 255);
  }
}

void sendChannelDataToSerial() {
  Serial.print("SSR relayArduino's id is ");
  Serial.print(myId);
  Serial.println();
  Serial.println("SSR relays ready to work");
  Serial.print("Relays use channels ");
  Serial.print(dmxBeginChannel);
  Serial.print(" - ");
  Serial.print(dmxBeginChannel+(1));
  Serial.println();
}
