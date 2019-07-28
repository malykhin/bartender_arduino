#include <EEPROM.h>
#include "ArduinoJson.h"
#include "AccelStepper.h"
#include "Servo.h"

#include "constants.h"
#include "custom_types.h"

AccelStepper stepper(AccelStepper::DRIVER, DIR, STEP);
Servo servo;

settings_t settings;
bool stringComplete = false;
String inputString = "";
int charCounter = 0;

void setup() {
  pinMode(NOT_LIMIT, INPUT);
  pinMode(NOT_GLASS_SENSOR, INPUT);

  EEPROM.get(SETTINGS_ADDR, settings);

  Serial.begin(BAUD_RATE);
  Serial.println(RESPONSE_READY);
}

void loop() {
  if (stringComplete) {
    manage();
    inputString = "";
    stringComplete = false;
    delay(200);
  }
}

void manage () {
  DynamicJsonDocument message(BUFFER_SIZE);
  deserializeJson(message, inputString);

  const char* command = message["command"];

  if ((String)command == (String)GET_SETTINGS) {
    DynamicJsonDocument response(BUFFER_SIZE);

    EEPROM.get(SETTINGS_ADDR, settings);

    response["zeroSpeed"] = settings.zeroSpeed;
    response["zeroAccel"] = settings.zeroAccel;
    response["maxStroke"] = settings.maxStroke;
    response["speed"] = settings.speed;
    response["accel"] = settings.accel;
    response["dozerOn"] = settings.dozerOn;
    response["dozerOff"] = settings.dozerOff;
    response["dozerCycleDelay"] = settings.dozerCycleDelay;

    serializeJson(response, Serial);
    Serial.println();
    return;
  }

  if ((String)command == (String)SET_SETTINGS) {
    settings.zeroSpeed = message["zeroSpeed"];
    settings.zeroAccel = message["zeroAccel"];
    settings.maxStroke = message["maxStroke"];
    settings.speed = message["speed"];
    settings.accel =  message["accel"];
    settings.dozerOn = message["dozerOn"];
    settings.dozerOff = message["dozerOff"];
    settings.dozerCycleDelay = message["dozerCycleDelay"];

    EEPROM.put(SETTINGS_ADDR, settings);

    Serial.println(RESPONSE_OK);
    return;
  }

  if ((String)command == (String)MOVE_TO) {
    moveTo(settings.speed, settings.accel, message["position"]);
    Serial.println(RESPONSE_OK);
    return;
  }

  if ((String)command == (String)PUSH_DOZER) {
    pushDozer(settings.dozerOn, settings.dozerOff, settings.dozerCycleDelay);
    Serial.println(RESPONSE_OK);
    return;
  }

  if ((String)command == (String)RESET) {
    servo.attach(SERVO);
    servo.write(settings.dozerOff);
    delay(SERVO_DELAY);
    servo.detach();

    const bool isAtHome = zeroAxis(settings.zeroSpeed, settings.zeroAccel, settings.maxStroke);
    if (isAtHome) {
      Serial.println(RESPONSE_OK);
    } else {
      Serial.println(RESPONSE_NOT_HOMED);
    }
    return;
  }

  if ((String)command == (String)GET_STATUS) {
    DynamicJsonDocument response(BUFFER_SIZE);

    const bool isAtHome = !digitalRead(NOT_LIMIT);
    const bool isGlassInserted = !digitalRead(NOT_GLASS_SENSOR);

    response["status"] = isAtHome && isGlassInserted ? "OK" : "ERROR";
    response["isAtHome"] = isAtHome;
    response["isGlassInserted"] = isGlassInserted;

    serializeJson(response, Serial);
    Serial.println();
  }
}

void serialEvent() {
  while ( Serial.available() ) {
    char inChar = (char) Serial.read();
    if (charCounter == 0 && inChar != '{') {
      continue;
    } else if (inChar == '\n') {
      charCounter = 0;
      stringComplete = true;
    } else {
      inputString += inChar;
      charCounter++;
    }
  }
}
