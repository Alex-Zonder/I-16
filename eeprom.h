#include <EEPROM.h>

class Eeprom {
  public:
  void WriteByte(int address, char value){
    EEPROM.write(address, value);
  }
  void WriteInt(int p_address, int p_value){
    EEPROM.write(p_address, ((p_value >> 0) & 0xFF));
    EEPROM.write(p_address+1, ((p_value >> 8) & 0xFF));
  }
  void WriteLong(int address, long value) {
    byte four = (value & 0xFF);
    byte three = ((value >> 8) & 0xFF);
    byte two = ((value >> 16) & 0xFF);
    byte one = ((value >> 24) & 0xFF);
    EEPROM.write(address, four);
    EEPROM.write(address + 1, three);
    EEPROM.write(address + 2, two);
    EEPROM.write(address + 3, one);
  }

  char ReadByte(int address){
    char value = EEPROM.read(address);
    if (value == -1) value = 0;
    return value;
  }
  unsigned int ReadInt(int p_address) {
    unsigned int value = 
      ((EEPROM.read(p_address) << 0) & 0xFF) + 
      ((EEPROM.read(p_address+1) << 8) & 0xFF00);
    if (value == -1) value = 0;
    return value;
  }
  unsigned long ReadLong(int address) {
    char four = EEPROM.read(address);
    char three = EEPROM.read(address + 1);
    char two = EEPROM.read(address + 2);
    char one = EEPROM.read(address + 3);
    unsigned long value = 
      ((four << 0) & 0xFF) + 
      ((three << 8) & 0xFFFF) + 
      ((two << 16) & 0xFFFFFF) + 
      ((one << 24) & 0xFFFFFFFF);
    if (value == -1) value = 0;
    return value;
  }
};
Eeprom eeprom;
