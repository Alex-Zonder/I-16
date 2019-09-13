//                  V E R S I O N                  //
char version[] = "2.b.0";

//                  P O R T S                  //
char rw485 = A3;
char addrBut = A5, addrLed = A4;
char buzzer_pin = A2;
//char inputs[16]={2,3,4,5,6,7,8,9,10,11,12,13,A0,A1,A6,A7};

//                  Main Settings                  //
char devName[] = "I16";
char modNum = 0;
char usartSoftEnable = 0;

//                  Includes                  //
#include "port.h"
#include "tone_blink.h"
#include "eeprom.h"
#include "usart.h"

//                  Ports Settings                  //
Port port[16];
struct Port_setts {
  //char pin[16];
  char pin[16] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, A0, A1, A6, A7};
  int mode[16] = {OUTPUT, OUTPUT, OUTPUT, INPUT, INPUT, INPUT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  //     I N P U T S   S E T T S     //
  char inputsToOuts[16] =              {0,0,0,0,1,2,0,0,0,0,0,0,0,0,1,2};
  /*char inputsToOutsInvers[16] =        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  char inputsToOutsButSw[16] =         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  char inputsToOutsAction[16] =        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  //     O U T P U T S   S E T T S     //
  char inputsToOutsIgnoreByPort[12] =  {0,0,0,0,0,0,0,0,0,0,0,0,0};
  char inputsToOutsIgnoreByState[12] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
  char outputsReturnTo[12] =           {0,0,0,0,0,0,0,0,0,0,0,0,0};
  int outputsReturnAfter[12] =         {0,0,0,0,0,0,0,0,0,0,0,0,0};*/
};
Port_setts port_setts;





//------------------------------------------------//
//                  Setup - Init                  //
//------------------------------------------------//
#include "main.h"
void setup() { 
  //          Blink / BEEP          //
  buzzer.Init(buzzer_pin);
  blinker.Init(addrLed);
  blinker.Blink(1,100);

  //          Addr But/Led          //
  pinMode(addrLed,OUTPUT);
  pinMode(addrBut,INPUT_PULLUP);

  //          Erase eeprom by AddrBut          //
  if (!digitalRead(addrBut)) {
    buzzer.Tone(330,50);
    blinker.Blink(5,100);
    int xEe=0;
    for (int x=0; x<8; x++) {
      xEe=x*128;
      for (int y=xEe; y < 128+xEe ; y++) eeprom.WriteByte(y,-1);
      blinker.Blink(1,25);
    }
    blinker.Blink(2,100);
  }

  //   - - -   R E A D   E E P R O M   - - -   //
  // ModNum: 0
  modNum = eeprom.ReadByte(0);
  // Usart Speed: 1 - 4
  usart.uSpeed = eeprom.ReadLong(1);
  if (usart.uSpeed == 0)
    usart.uSpeed = 9600;
  // Usart Gate: 5
  usartSoftEnable = eeprom.ReadByte(5);

  //          Init Ports          //
  for (int x=0; x<sizeof(port_setts.pin); x++) {
    int p_state = -1;
    if (port_setts.mode[x] == OUTPUT) p_state = 1;
    port[x].Init(port_setts.pin[x], port_setts.mode[x]);
  }

  //          Init Usart          //
  usart.Init(usart.uSpeed);
  //          Init SoftUsart          //
  if (usartSoftEnable){
    softUsart.Init(port_setts.pin[10], port_setts.pin[11], 9600);
  }
  //          Init UsartManager          //
  usartManager.Init(rw485);

  //          Start           //
  usartManager.Send("Loaded");
  buzzer.Tone(440,50);
  delay(100);
  buzzer.Tone(440,50);
}





//------------------------------------------------//
//                   Main While                   //
//------------------------------------------------//
char a2dTim = 0;
int addrButTim = 0;
void loop() {
  // Checking inputs (1-12) (Digital) //
  for (int x=0; x<12; x++){
    if (port_setts.mode[x] == INPUT && port[x].IsChanged()) {
      InputChanged (x);
    }
  }

  // Checking inputs (13-16) (Analog) //
  if (!a2dTim){
    a2dTim=100;
    for (int x=12; x<16; x++){
      if (port_setts.mode[x] == INPUT && port[x].IsChanged()) {
        InputChanged (x);
      }
    }
  }
  else a2dTim--;

  //   A D D R E S S   //
  if (!digitalRead(addrBut)) {
    addrButTim++;
    if (addrButTim==15001) {
      // Disable usart !!!
      usart.Stop();
      
      buzzer.Tone(330,50);
      modNum=0;
      EEPROM.write(0,modNum);
      blinker.BlinkNum(modNum);
    }
    if (addrButTim==25001) {
      addrButTim=15002;
      modNum++;
      EEPROM.write(0,modNum);
      blinker.BlinkNum(modNum);
    }
  }
  else if (digitalRead(addrBut) && addrButTim) {
    if (addrButTim<15000) {
      blinker.BlinkNum(modNum);
    }
    else {
      // Enable usart !!!
      usart.Start();
      
      // BEEP
      buzzer.Tone(440,50);
      delay(100);
      buzzer.Tone(440,50);
    }
    addrButTim=0;
  }
}
