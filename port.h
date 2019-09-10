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
      this->pin_state = digitalRead(this->pin);
    }
    else {
      this->pin_state = p_state;
      this->Write(this->pin_state);
    }
  }


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


  /*      R E A D      */
  bool Read () {
    return digitalRead(this->pin);
  }

  /*bool ReadMode () {
    return this->pin_mode;
  }*/

  bool IsChanged () {
    bool changed = false;
    if (changed_time + change_time_out < millis()) {
      if (this->pin_state != digitalRead(this->pin)) {
        changed = true;
        this->pin_state = digitalRead(this->pin);
        changed_time = millis();
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
};
