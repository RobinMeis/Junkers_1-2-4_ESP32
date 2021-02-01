float targetTemperature = 18;
float currentTemperature = targetTemperature;
bool standby = true;
long nextSet = 0;

void setTargetTemperature(float target) {
  targetTemperature = target;
}

void setCurrentTemperature(float current) {
  currentTemperature = current;
}

float getTargetTemperature() {
  return targetTemperature;
}

float getCurrentTemperature() {
  return currentTemperature;
}

bool isOn() {
  return !standby;
}

bool isStandy() {
  return standby;
}

void setStandby() {
  standby = true;
}

void setOn() {
  standby = false;
}

bool getToSet() {
  if (nextSet > millis()) return false;
  else return true;
}

void setNextSet(long add_ms) { //Queue next controller set in ms (specify time to wait until next set)
  if (add_ms == 0) nextSet = 0;
  else nextSet = millis() + add_ms;
}

int getPower() {
  float delta = currentTemperature - targetTemperature;
  if (!standby) { //On
    if (delta >= 1.5) { //Off, 0%
      return 0;
    } else if (delta < 2 && delta > 0.5) { //Heat a little, 54%
      return 13;
    } else if (delta >= -0.5 && delta <= 0.5) { //Heat a little more, try to keep temperature, 70%
      return 17;
    } else { //Heat full, 100%
      return 24;
    }
  } else { //Standby
    if (currentTemperature >= targetTemperature) { //Off, 0%
      return 0;
    } else if (delta > -1) { //Heat a little, 54%
      return 13;
    } else if (delta > -2) { //Heat a little more, try to keep temperature, 70%
      return 17;
    } else { //Heat full, 100%
      return 24;
    }
  }
}
