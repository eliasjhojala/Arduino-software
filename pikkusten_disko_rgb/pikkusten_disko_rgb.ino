#include <EEPROM.h>

#include <Adafruit_NeoPixel.h>

#define PIN 22
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

int transPins[4] = { 3, 4, 2, 5 };
int dmxBeginChannel = 10;

int myId = 2;

int value = 0;
int channel;


int digitalRed;
int digitalGreen;
int digitalBlue;
int digitalLength;
int digitalUpper;
int digitalLower;

void setup() {
    strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  dmxBeginChannel = EEPROM.read(1);
  for(int i = 0; i <= 10; i++) {
    pinMode(i+2, OUTPUT);
  }
  //Tell to serial monitor what channels etc. you are using
  Serial.begin(9600);
  printInfoToSerial();
}
void loop() {
   int c;

  while(!Serial.available());
  c = Serial.read();
  if ((c>='0') && (c<='9')) {
    value = 10*value + c - '0';
  } else {
    if (c=='c') channel = value;
    else if (c=='w') {
      if(channel-dmxBeginChannel <= sizeof(transPins)/sizeof(int)-1 && channel-dmxBeginChannel >= 0) {
        analogWrite(transPins[channel-dmxBeginChannel], value);
      }
      else if(channel-dmxBeginChannel <= sizeof(transPins)/sizeof(int)-1+5 && channel-dmxBeginChannel >= sizeof(transPins)/sizeof(int)-1) {
        if(channel-dmxBeginChannel == sizeof(transPins)/sizeof(int)-1+1) {
          digitalRed = value;
        }
        if(channel-dmxBeginChannel == sizeof(transPins)/sizeof(int)-1+2) {
          digitalGreen = value;
        }
        if(channel-dmxBeginChannel == sizeof(transPins)/sizeof(int)-1+3) {
          digitalBlue = value;
        }
        if(channel-dmxBeginChannel == sizeof(transPins)/sizeof(int)-1+4) {
          digitalUpper = map(value, 0, 255, 0, 50);
        }
        if(channel-dmxBeginChannel == sizeof(transPins)/sizeof(int)-1+5) {
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
        printInfoToSerial();
      }
    }
    value = 0;
  }
}
void printInfoToSerial() {
  Serial.print("SSR rgb Arduino's id is ");
  Serial.print(myId);
  Serial.println();
  Serial.println("rgb Arduino ready to work");
  Serial.print("RGB Arduino use channels ");
  Serial.print(dmxBeginChannel);
  Serial.print(" - ");
  Serial.print(dmxBeginChannel+(sizeof(transPins)/sizeof(int))-1+4);
  Serial.println();
  Serial.print(dmxBeginChannel+0);
  Serial.println(" analog red");
  Serial.print(dmxBeginChannel+1);
  Serial.println(" analog green");
  Serial.print(dmxBeginChannel+2);
  Serial.println(" analog blue");
  Serial.print(dmxBeginChannel+3);
  Serial.println(" analog white");
  Serial.print(dmxBeginChannel+4);
  Serial.println(" digital red");
  Serial.print(dmxBeginChannel+5);
  Serial.println(" digital green");
  Serial.print(dmxBeginChannel+6);
  Serial.println(" digital blue");
  Serial.print(dmxBeginChannel+7);
  Serial.println(" digital upper");
  Serial.print(dmxBeginChannel+8);
  Serial.println(" digital lower");
}
