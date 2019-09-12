class Buzzer {
  public:
  /*      I N I T      */
  void Init (char p) {
    this->pin = p;
    this->Tone(440, 50);
  }

  void Tone (int freq, int tim) {
    tone(this->pin, freq, tim);
  }

  private:
  char pin;
};
Buzzer buzzer;



class Blinker {
  public:
  /*      I N I T      */
  void Init (char p) {
    this->pin = p;
    pinMode(this->pin, OUTPUT);
  }

  void Blink (char times, int sleep) {
    for (int x=0; x<times; x++) {
      digitalWrite(addrLed,1);
      delay(sleep/2);
      digitalWrite(addrLed,0);
      delay(sleep/2);
    }
  }

  void BlinkNum (int num) {
    if (num == 0)
      this->Blink(3, 200);
    else {
      this->Blink(num/10, 1000);
      this->Blink(num%10, 400);
    }
  }
  
  private:
  char pin;
};
Blinker blinker;
