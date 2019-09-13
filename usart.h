//------------------------------------------------//
//                     Usart                      //
//------------------------------------------------//
class Usart {
  public:
  long uSpeed;
  bool enabled = false;

  void Init (long uSpeed) {
    this->uSpeed = uSpeed;
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

  void Send (char * data) {
      Serial.print(data);
  }

  char Read () {
    if (Serial.available())
      return Serial.read();
    else
      return false;
  }
};
Usart usart;




//------------------------------------------------//
//               S O F T   U S A R T              //
//------------------------------------------------//
#include <SoftwareSerial.h>
class SoftUsart {
  public:
  char pin_rx;
  char pin_tx;
  int uSpeed = 9600;
  bool enabled = false;

  void Init (char pinRx, char pinTx, int uSpeed) {
    this->pin_rx = pinRx;
    this->pin_tx = pinTx;
    this->uSpeed = uSpeed;
    // Init Arduino SoftwareSerial (Resave) //
    SoftwareSerial SoftSerial(this->pin_rx, this->pin_tx); // RX, TX
    SoftSerial.begin(this->uSpeed);
    // Init Arduino SoftSerial (Transmit) //
    pinMode(this->pin_tx, 1);
    digitalWrite(this->pin_tx, 1);
  }

  void SendBit (bool myBit) {
    digitalWrite (this->pin_tx, myBit);
    delayMicroseconds(96);
  }
  void SendByte (byte myByte) {
    this->SendBit (0);  // Strat bit
    for (int i = 0; i < 8; i++){
      byte sendByte = myByte >> i;
      //if (sendByte & 0x01) this->SendBit (1);
      //else this->SendBit (0);
      this->SendBit (sendByte & 0x01);
    }
    this->SendBit (1);  // Stop bit
  }
  void Send (char * data) {
    for (char x=0; x<strlen(data); x++)
      this->SendByte(data[x]);
  }
};
SoftUsart softUsart;




//------------------------------------------------//
//                 Usarts manager                 //
//------------------------------------------------//
class UsartManager {
  public:
  bool enterEnable = true;
  struct Data {
    char resaved[50];
    char transmit[50];
  };
  Data data;

  void Init (char rw) {
    this->rw = rw;
    pinMode(this->rw, OUTPUT);
    digitalWrite(this->rw, LOW);
  }

  void Send (char * data, bool capsule = true) {
    if (strlen(data) > 1) {
      // Enable transmit-mode to rs-485 //
      digitalWrite(this->rw, HIGH);

      // Capsule in dataToRs //
      if (capsule) {
        char capsuled[strlen(data) + 5];
        sprintf(capsuled, "#%c%c%s;", ((int)modNum/10+0x30), ((int)modNum%10)+0x30,data);
        sprintf(this->data.transmit, "%s", capsuled);
      }
      else sprintf(this->data.transmit,"%s",data);
      // Adding Enter //
      if (this->enterEnable)
        sprintf(this->data.transmit,"%s\n",this->data.transmit);        

      // Send to Usart //
      if (usart.enabled) {
        usart.Send(this->data.transmit);
        // Send to Usart Soft //
        if (softUsart.enabled) {
          softUsart.Send(this->data.transmit);
        }
        // RW Timeout //
        else {
          delay(strlen(this->data.transmit)+1);
          if (strlen(this->data.transmit)>15) delay(1);
        }
      }

      // Send to Usart Soft //
      else if (softUsart.enabled) {
        softUsart.Send(this->data.transmit);
      }

      // Enable resave-mode to rs-485 //
      digitalWrite(this->rw, LOW);
    }
  }

  char * Read () {
    if (usart.enabled) {
      char bt = usart.Read();
    }
  }
  

  private:
  char rw;
};
UsartManager usartManager;
