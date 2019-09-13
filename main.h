//          Main variables         //




//          Garbage         //
//memcpy(port_setts.pin, inputs, sizeof(inputs));
//port[0].Init(port_setts.pin[0], OUTPUT, 1);
//sizeof(port_setts.pin)



//---------------------------------------------------//
//                 String from eeprom                //
//---------------------------------------------------//
int startOfEeComs = 256;
void SendComandFromEe (char comNum) {
  int comEeStart = startOfEeComs+((comNum-1)*14);
  char value[2]="";
  char eeDataStr[16]="";
  sprintf(eeDataStr,"");
  for (char x=0; x<14 && value[0]!=-1; x++) {
    sprintf(value,"%c",EEPROM.read(comEeStart+x));
    if (value[0]!=-1) strcat(eeDataStr,value);
  }
  usartManager.Send(eeDataStr, false);
}
char CompareComandWithEe (char * data, int comEeStart) {
  char dataIsCompare=0;
  char eeByte=0;
  for (char x=0; x<14 && dataIsCompare==0; x++) {
    eeByte=EEPROM.read(comEeStart+x);
    if (eeByte==';' && data[x]==':') eeByte=':';
    if (eeByte!=data[x]) dataIsCompare=-1;
    else if (data[x]==';') dataIsCompare=1;
  }
  return dataIsCompare;
}



//---------------------------------------------------//
//                   Convert bytes                   //
//---------------------------------------------------//
int GetIntFromBytes(char * data, char from, char bytes) {
  int num=0;
  for (int x=0; x<bytes && data[from+x]!=';'; x++) {
    num=num*10;
    num=num+((char)data[from+x] - 0x30);
  }
  return num;
}
long GetLongFromBytes(char * data, char from, char bytes) {
  long num=0;
  for (int x=0; x<bytes && data[from+x]!=';'; x++) {
    num=num*10;
    num=num+(int)(data[from+x] - 0x30);
  }
  return num;
}

char numCh[6];
char * ConvIntToBytes(int val, char bytes) {
  if (bytes==1) {
    if (val==0) sprintf(numCh,"0");
    else if (val<=9) sprintf(numCh,"%i",val);
  }
  if (bytes==2) {
    if (val==0) sprintf(numCh,"00");
    else if (val<=99) sprintf(numCh,"%i%i",val/10,val%10);
  }
  if (bytes==3) {
    if (val==0) sprintf(numCh,"000");
    else if (val<=999) sprintf(numCh,"%i%i%i",val/100,val%100/10,val%10);
  }
  else if (bytes==4) {
    if (val==0) sprintf(numCh,"0000");
    else sprintf(numCh,"%i%i%i%i",val/1000,val%1000/100,val%100/10,val%10);
  }
  return numCh;
}



//------------------------------------------------//
//                     Inputs                     //
//------------------------------------------------//
void InputChanged (int num){
  buzzer.Tone(330, 50);
  
  sprintf(usartManager.data.transmit, "I%s:%i", ConvIntToBytes(num+1,2), port[num].pin_state);
  usartManager.Send(usartManager.data.transmit);

  port[port_setts.inputsToOuts[num]].Change();
  //CheckLinkToPort (num,0);
}
