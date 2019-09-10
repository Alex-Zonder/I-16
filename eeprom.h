#include <EEPROM.h>

//   Char   V A L U E   //
void EeWrite(int address, char value) {
  EEPROM.write(address, value);
}
char EeRead(int address) {
  char value=EEPROM.read(address);
  if (value==-1) value=0;
  return value;
}


//   I N T   V A L U E   //
void EeWriteInt(int p_address, int p_value) {
  byte oneByte[3];
  oneByte[0] = ((p_value >> 0) & 0xFF);
  oneByte[1] = ((p_value >> 8) & 0xFF);
  EEPROM.write(p_address, oneByte[0]);
  EEPROM.write(p_address+1, oneByte[1]);
}
unsigned int EeReadInt(int p_address) {
  byte oneByte[3];
  oneByte[0] = EEPROM.read(p_address);
  oneByte[1] = EEPROM.read(p_address+1);
  return 
    ((oneByte[0] << 0) & 0xFF) + 
    ((oneByte[1] << 8) & 0xFF00);
}


//   L O N G   V A L U E   //
void EeWriteLong(int address, long value) {
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}
unsigned long EeReadLong(int address) {
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);
  
  //Return the recomposed long by using bitshift.
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}
