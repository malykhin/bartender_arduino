void moveTo(int speed, int accel, int position) {
  stepper.setMaxSpeed(speed);
  stepper.setAcceleration(accel);
  stepper.moveTo(position);

  while (stepper.isRunning()) {
    stepper.run();
  }
}

bool zeroAxis(int speed, int accel, int stroke) {
  stepper.setMaxSpeed(speed);
  stepper.setAcceleration(accel);
  stepper.move(stroke);

  while (stepper.isRunning()) {
    stepper.run();
    if ( !digitalRead(NOT_LIMIT) ) {
      break;
    }
  }

  stepper.disableOutputs ();
  stepper.stop ();
  stepper.setCurrentPosition(0);

  return (bool)!digitalRead(NOT_LIMIT);
}


void pushDozer(int on, int off, int cycleDuration) {
  servo.attach(SERVO);
  servo.write(on);
  delay(cycleDuration);
  servo.write(off);
  delay(SERVO_DELAY);
  servo.detach();
}

void setDozerPosition(int pos) {
  servo.attach(SERVO);
  servo.write(pos);
  delay(SERVO_DELAY);
  servo.detach();
}
