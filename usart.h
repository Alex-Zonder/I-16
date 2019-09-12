//________   U S A R T   ________//
class Usart {
  public:
  char rw;
  long uSpeed;
  bool enabled = false;
  bool enterEnable = false;
  struct Data {
    char resaved[50];
    char transmit[50];
  };
  Data data;

  void Init (long uSpeed, char rw) {
    this->uSpeed = uSpeed;
    this->rw = rw;
    pinMode(this->rw, OUTPUT);
    digitalWrite(this->rw, LOW);
    this->Start();
  }

  void Start () {
    Serial.begin(this->uSpeed);
    this->enabled = true;
  }
  void Stop () {
    Serial.end();
    this->enabled = false;
  }

  void Send (char * data, char capsule = 1) {
    if (strlen(data) > 1) {
      // Enable transmit-mode to rs-485 //
      digitalWrite(this->rw, HIGH);

      // Capsule in dataToRs //
      if (capsule == 1) {
        char capsuled[strlen(data) + 5];
        sprintf(capsuled, "#%c%c%s;", ((int)modNum/10+0x30), ((int)modNum%10)+0x30,data);
        sprintf(this->data.transmit, "%s", capsuled);
      }
      else sprintf(this->data.transmit,"%s",data);

      // Send to Usart //
      if (this->enabled) {
        Serial.print(this->data.transmit);
        if (this->enterEnable) Serial.print("\n");        
      }      

      // Enable resave-mode to rs-485 //
      delay(strlen(this->data.transmit)+1);
      if (strlen(this->data.transmit)>15) delay(1);
      digitalWrite(this->rw, LOW);
    }
  }
};
Usart usart;




//________   S O F T   U S A R T   ________//
#include <SoftwareSerial.h>
class SoftUsart {
  public:
  char pin_rx;
  char pin_tx;
  int uSpeed = 9600;
  bool enabled = false;
  bool enterEnable = false;
  struct Data {
    char resaved[50];
    char transmit[50];
  };
  Data data;

  void Init (char pinRx, char pinTx, int uSpeed) {
    this->pin_rx = pinRx;
    this->pin_tx = pinTx;
    this->uSpeed = uSpeed;
    // Init Arduino SoftwareSerial (Resave) //
    SoftwareSerial SoftSerial(this->pin_rx, this->pin_tx); // RX, TX
    // Init Arduino SoftSerial (Transmit) //
    pinMode(this->pin_tx,1);
    digitalWrite(this->pin_tx,1);
  }
};
SoftUsart softUsart;
