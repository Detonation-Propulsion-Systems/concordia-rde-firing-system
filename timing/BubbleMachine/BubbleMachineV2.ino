int greenLed = 13;
int yelLed = 12;
int redLed = 11;
int valvePin = 10; //Actuator signal pin.
int bubblePin = 9; //Ignition signal pin.
int noPin = 8; //Normally open solenoid valves pin.

//Trigger remote pins

int forwardPin = 2;
int rewindPin = 4;
int trigger = 3;

bool checkGreen = LOW;
bool checkYel = LOW;
bool checkRed = LOW;

int noDelay = 500; //Delay between the NO valves closing and the NC valves & actuators opening (SUBJECT TO CHANGE).
int triggerTime = 10620; //Trigger length.
int ignitionTime = 420; //Ignition delay after the start of the trigger.

void setup() {

  pinMode(greenLed, OUTPUT);
  pinMode(yelLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(bubblePin, OUTPUT);
  pinMode(valvePin, OUTPUT);
  pinMode(noPin, OUTPUT);

  pinMode(forwardPin, INPUT);
  pinMode(rewindPin, INPUT);
  pinMode(trigger, INPUT);
}

//LED colour code
//Green: Safe-initial step
//Yellow: Intermediate step (also safe)
//Red: Intermediate step (also safe)
//Red + Green: Firing
//Yellow + Red: Unexpected trigger activation. Firing system frozen until trigger switched off.

void unexpectedTrigger() { //Freezes the timing Arduino until the trigger is switched off.

  PORTB = B00011101;
}

void manualAbort() { //Disarms the system by resetting all control pins to their initial, safe value.

  PORTB = B00100101;

  checkRed = LOW;
  checkYel = LOW;
  checkGreen = LOW;
}

void loop() {

  PORTB = B00100101;

  while (digitalRead(trigger) == HIGH) { //Timing Arduino can't advance if the trigger is on.

    unexpectedTrigger();
  }

  if (digitalRead(forwardPin) == HIGH) {

    checkGreen = HIGH;
    delay(250);
  }

  while (checkGreen == HIGH) {

    PORTB = B00010101;

    if (digitalRead(trigger) == HIGH) {

      unexpectedTrigger();
    }

    if (digitalRead(forwardPin) == HIGH) {

      checkYel = HIGH;
      delay(250);
    }
    if (digitalRead(rewindPin) == HIGH) {

      checkGreen = LOW;
      delay(250);
    }

    while (checkYel == HIGH) {

      PORTB = B00001101;

      if (digitalRead(trigger) == HIGH) {

        checkRed = HIGH;
        delay(250);
      }
      if (digitalRead(rewindPin) == HIGH) {

        checkYel = LOW;
        delay(250);
      }

      if (checkRed == HIGH) {

        PORTB = B00101100; //The NO solenoid valves are closed first.

        delay(noDelay);

        //The rewind button on the remote has to be held down for manualAbort to take effect.

        if (digitalRead(rewindPin) == HIGH) {

          manualAbort();
        }

        PORTB = B00101000; //The NC solenoid valves and actuators are now open.

        if (digitalRead(rewindPin) == HIGH) {

          manualAbort();
        }

        if (checkRed == HIGH) {

          delay(ignitionTime); //Wait before ignition.

          if (digitalRead(rewindPin) == HIGH) {

            manualAbort();
          }

          PORTB = B00101010; //The ignition signal is sent.

          if (digitalRead(rewindPin) == HIGH) {

            manualAbort();
          }

          delay(triggerTime - ignitionTime);

          //After the test time is over, reset all pins to their original state by calling the manualAbort function.

          manualAbort();
        }
      }
    }
  }
}
