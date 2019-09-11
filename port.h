class Port {
  public:
  /*      I N I T      */
  void Init (char p, char p_mode, char p_state = -1) {
    this->pin = p;
    this->pin_mode = p_mode;
    pinMode(this->pin, this->pin_mode);
    // Maybe to output only //
    delay(1);
    if (p_state == -1) {
      this->pin_state = this->Read();
    }
    else {
      this->pin_state = p_state;
      this->Write(this->pin_state);
    }
  }


  /*      R E A D      */
  int ReadAnalog () {
    return (int)((double)(analogRead(this->pin)-92)*1.0987);
  }
  bool Read () {
    int state;
    if (this->pin != A6 && this->pin != A7)
      state = digitalRead(this->pin);
    else {
      state = this->ReadAnalog();
      if (state <= this->analog.min_level)
        state = 0;
      else
        state = 1;
    }
    return state;
  }

  /*bool ReadMode () {
    return this->pin_mode;
  }*/


  /*      W R I T E      */
  bool Write (char state) {
    bool changed = false;
    if (state != this->Read()) {
      digitalWrite(this->pin, state);
      changed = true;
      this->pin_state = state;
    }
    return changed;
  }

  bool Change () {
    digitalWrite(this->pin, !digitalRead(this->pin));
    return digitalRead(this->pin);
  }


  /*      I S   C H A N G E D      */
  bool IsChanged () {
    bool changed = false;
    if (changed_time + change_time_out < millis()) {
      char state = this->Read();
      if (this->pin_state != state) {
        changed = true;
        changed_time = millis();
        this->pin_state = state;
      }
    }
    return changed;
  }


  /*      V A R S      */
  private:
  char pin;
  char pin_mode;
  char pin_state;

  long change_time_out = 200;
  long changed_time;

  struct Analog {
    int min_level = 512;
    int max_level = 512;
  };
  Analog analog;
};
