#include <EEPROM.h>
#include <DmxSimple.h>

int transPins[4] = { 10, 11, 12, 13 };
int dmxBeginChannel = 10;

int myId = 2;

int value = 0;
int channel;

void setup() {

  dmxBeginChannel = EEPROM.read(1);
  for(int i = 4; i <= 10; i++) {
    pinMode(i+2, OUTPUT);
  }
  //Tell to serial monitor what channels etc. you are using
  Serial.begin(115200);
  printInfoToSerial();
  
  DmxSimple.usePin(4);
  DmxSimple.maxChannel(128);
  
  // Set shield to output mode
  pinMode (2, OUTPUT);
  digitalWrite (2, HIGH);
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
      DmxSimple.write(channel, value);
      if(channel-dmxBeginChannel <= sizeof(transPins)/sizeof(int)-1 && channel-dmxBeginChannel >= 0) {
        analogWrite(transPins[channel-dmxBeginChannel], value);
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
  Serial.print(dmxBeginChannel+(sizeof(transPins)/sizeof(int))-1);
  Serial.println();
  Serial.print(dmxBeginChannel+0);
  Serial.println(" analog red");
  Serial.print(dmxBeginChannel+1);
  Serial.println(" analog green");
  Serial.print(dmxBeginChannel+2);
  Serial.println(" analog blue");
  Serial.print(dmxBeginChannel+3);
  Serial.println(" analog white");
}
