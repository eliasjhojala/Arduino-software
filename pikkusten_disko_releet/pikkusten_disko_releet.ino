#include <Servo.h>
Servo tiltServo;

#include <EEPROM.h>


#include <Adafruit_NeoPixel.h>

#define PIN 12
Adafruit_NeoPixel strip = Adafruit_NeoPixel(44, PIN, NEO_GRB + NEO_KHZ800);

int relePins[10] = { 2, 3, A0, A1, A2, A3, A5, A5, A6, A7 };
int dmxBeginChannel = 10;

int mekRelePins[3] = { 5, 6, 7 };
int motorShieldPins[4] = { 10, 11, 12, 13 };

int myId = 1;

int strobeOutputValue = 0;
long previousMillis = 0;        // will store last time LED was updated
long interval = 1000;           // interval at which to blink (milliseconds)

int value = 0;
int channel;

int panSpeed = 255;



int digitalRed;
int digitalGreen;
int digitalBlue;
int digitalLength;
int digitalUpper;
int digitalLower;


void setup() {
  analogWrite(6, 255);
  analogWrite(7, 255);
  dmxBeginChannel = EEPROM.read(1);
  for(int i = 0; i <= 10; i++) {
    pinMode(i+2, OUTPUT);
  }
  //Tell to serial monitor what channels etc. you are using
  Serial.begin(9600);
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
      if(channel-dmxBeginChannel == 13) {
        tilt(value);
      }
      if(channel-dmxBeginChannel == 14 && value > 100) {
        panUp();
      }
      if(channel-dmxBeginChannel == 15 && value > 100) {
        panDown();
      }
      if(channel-dmxBeginChannel == 16) {
        panSpeed = 255-value;
      }
      if((channel-dmxBeginChannel == 15 || channel-dmxBeginChannel == 14) && value < 100) {
        panStop();
      }
      if(channel-dmxBeginChannel == 17) {
        analogWrite(mekRelePins[0], value);
      }
      if(channel-dmxBeginChannel == 18) {
        analogWrite(mekRelePins[1], 255-value);
      }
      if(channel-dmxBeginChannel == 19) {
        analogWrite(mekRelePins[2], 255-value);
      }
      if(channel-dmxBeginChannel <= 11 && channel-dmxBeginChannel >= 7) {
        if(channel-dmxBeginChannel == 7) {
          digitalRed = value;
        }
        if(channel-dmxBeginChannel == 8) {
          digitalGreen = value;
        }
        if(channel-dmxBeginChannel == 9) {
          digitalBlue = value;
        }
        if(channel-dmxBeginChannel == 10) {
          digitalUpper = map(value, 0, 255, 0, 50);
        }
        if(channel-dmxBeginChannel == 11) {
          digitalLower = map(value, 0, 255, 0, 50);
        }
        for(int i = 0; i < digitalLower; i++) {
          strip.setPixelColor(i, 0, 0, 0);
        }
        for(int i = digitalLower; i < digitalUpper; i++) {
          strip.setPixelColor(i, digitalRed, digitalBlue, digitalGreen);
        }
        for(int i = digitalUpper; i < 50; i++) {
          strip.setPixelColor(i, 0, 0, 0);
        }
        strip.show();
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
  Serial.print(", ");
  Serial.print(dmxBeginChannel+(1));
  Serial.print(", ");
  Serial.print(dmxBeginChannel+(7));
  Serial.print(", ");
  Serial.print(dmxBeginChannel+(8));
  Serial.print(", ");
  Serial.print(dmxBeginChannel+(9));
  Serial.print(", ");
  Serial.print(dmxBeginChannel+(10));
  Serial.print(", ");
  Serial.print(dmxBeginChannel+(11));
  Serial.print(", ");

  
  Serial.print(dmxBeginChannel+(13));
  Serial.print(", ");
  Serial.print(dmxBeginChannel+(14));
  Serial.print(", ");
  Serial.print(dmxBeginChannel+(15));
  Serial.print(", ");
  Serial.print(dmxBeginChannel+(16));
  Serial.print(", ");
  Serial.print(dmxBeginChannel+(17));
  Serial.print(", ");
  Serial.print(dmxBeginChannel+(18));
  Serial.print(", ");
  Serial.print(dmxBeginChannel+(19));
  Serial.println();
}

void panUp() {
  analogWrite(motorShieldPins[0], panSpeed);
  analogWrite(motorShieldPins[1], 0);
}
void panDown() {
  analogWrite(motorShieldPins[1], panSpeed);
  analogWrite(motorShieldPins[0], 0);
}
void panStop() {
  analogWrite(motorShieldPins[0], 0);
  analogWrite(motorShieldPins[1], 0);
}
void tilt(int value) {
  tiltServo.write(constrain(map(value, 0, 255, 0, 180), 10, 170));
}
