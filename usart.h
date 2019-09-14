//------------------------------------------------//
//                     Usart                      //
//------------------------------------------------//
class Usart {
  public:
  long uSpeed;
  bool enabled = false;

  //      Init      //
  void Init (long uSpeed) {
    this->uSpeed = uSpeed;
    this->Start();
  }

  //      Start / Stop      //
  void Start () {
    Serial.begin(this->uSpeed);
    this->enabled = true;
  }
  void Stop () {
    Serial.end();
    this->enabled = false;
  }

  //      Send      //
  void Send (char * data) {
      Serial.print(data);
  }

  //      Avalible / Read      //
  bool Avalible () {
    return Serial.available();
  }
  char Read () {
    return Serial.read();
  }
};
Usart usart;




//------------------------------------------------//
//               S O F T   U S A R T              //
//------------------------------------------------//
#include <SoftwareSerial.h>
SoftwareSerial SoftSerial(12, 13); // RX, TX
class SoftUsart {
  public:
  char pin_rx;
  char pin_tx;
  int uSpeed = 9600;
  bool enabled = false;

  //      Init      //
  void Init (char pinRx, char pinTx, int uSpeed) {
    this->pin_rx = pinRx;
    this->pin_tx = pinTx;
    this->uSpeed = uSpeed;
    // Init Arduino SoftwareSerial (Resave) //
    SoftSerial.begin(this->uSpeed);
    // Init Arduino SoftSerial (Transmit) //
    pinMode(this->pin_tx, 1);
    digitalWrite(this->pin_tx, 1);
  }

  //      Start / Stop      //
  void Start () {
    SoftSerial.begin(this->uSpeed);
    this->enabled = true;
  }
  void Stop () {
    SoftSerial.end();
    this->enabled = false;
  }

  //      Send      //
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

  //      Avalible / Read      //
  bool Avalible () {
    return SoftSerial.available();
  }
  char Read () {
    return SoftSerial.read();
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
    char resaved[64];
    char transmit[64];
  };
  Data data;

  //      Init      //
  void Init (char rw) {
    this->rw = rw;
    pinMode(this->rw, OUTPUT);
    digitalWrite(this->rw, LOW);
  }

  //      Start / Stop      //
  void Start () {
    usart.Start();
    if (usartSoftEnable)
      softUsart.Start();
  }
  void Stop () {
    usart.Stop();
    if (usartSoftEnable)
      softUsart.Stop();
  }

  //      Send      //
  void Send (char * data, bool capsule = true) {
    if (strlen(data) > 1) {
      // Enable transmit-mode to rs-485 //
      digitalWrite(this->rw, HIGH);

      // Capsule in this->data.transmit //
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

  //      Avalible / Read      //
  bool Read () {
    char ch_counter = 0;
    bool command_readed = false;

    // Read Usart //
    if (usart.enabled && usart.Avalible()) {
      command_readed = false;
      ch_counter = 0;
      while (usart.Avalible() && !command_readed) {
        // Enable Rx led //
        digitalWrite(addrLed, HIGH);
        // Read byte //
        this->data.resaved[ch_counter] = usart.Read();
        // Waiting next byte //
        if (this->data.resaved[ch_counter] != ';') {
          for (int tim=0; !usart.Avalible() && tim<25; tim++) delayMicroseconds(100);
        }
        else command_readed = true;
        // Plus counter //
        ch_counter++;
        if (ch_counter >= 64) ch_counter = 0;
      }
    }

    // Read SoftUsart //
    if (softUsart.enabled && softUsart.Avalible()) {
      command_readed = false;
      ch_counter = 0;
      while (softUsart.Avalible() && !command_readed) {
        // Enable Rx led //
        digitalWrite(addrLed, HIGH);
        // Read byte //
        this->data.resaved[ch_counter] = softUsart.Read();
        // Waiting next byte //
        if (this->data.resaved[ch_counter] != ';') {
          for (int tim=0; !softUsart.Avalible() && tim<25; tim++) delayMicroseconds(100);
        }
        else command_readed = true;
        // Plus counter //
        ch_counter++;
        if (ch_counter >= 64) ch_counter = 0;
      }
    }

    // If this->data.resaved Is //
    if (ch_counter) {
      // Disable Rx led //
      digitalWrite(addrLed, LOW);
      // Erasing end of this->data.resaved //
      for (ch_counter; ch_counter<64; ch_counter++)
        this->data.resaved[ch_counter]='\0';
    }
    // Return //
    return command_readed;
  }
  

  private:
  char rw;
};
UsartManager usartManager;
