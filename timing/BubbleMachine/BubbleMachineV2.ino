int greenLed = 13;
int yelLed = 12;
int redLed = 11;
int valvePin = 10; //Actuator signal pin.
int bubblePin = 9; //Ignition signal pin.

//Trigger remote pins

int forwardPin = 2;
int rewindPin = 4;
int trigger = 3;

bool checkGreen = LOW;
bool checkYel = LOW;
bool checkRed = LOW;

int triggerTime = 10000; //Trigger length.
int ignitionTime = 420; //Ignition delay after the start of the trigger.

void setup() {

  pinMode(greenLed, OUTPUT);
  pinMode(yelLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(bubblePin, OUTPUT);
  pinMode(valvePin, OUTPUT);

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

  PORTB = B00011100;
}

void manualAbort() { //Disarms the system by resetting all control pins to their initial, safe value.

  PORTB = B00100100;

  checkRed = LOW;
  checkYel = LOW;
  checkGreen = LOW;
}

void loop() {

  PORTB = B00100100;

  while (digitalRead(trigger) == HIGH) { //Timing Arduino can't advance if the trigger is on.

    unexpectedTrigger();
  }

  if (digitalRead(forwardPin) == HIGH) {

    checkGreen = HIGH;
    delay(250);
  }

  while (checkGreen == HIGH) {

    PORTB = B00010100;

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

      PORTB = B00001100;

      if (digitalRead(trigger) == HIGH) {

        checkRed = HIGH;
        delay(250);
      }
      if (digitalRead(rewindPin) == HIGH) {

        checkYel = LOW;
        delay(250);
      }
      
      if (checkRed == HIGH) {

        while (digitalRead(rewindPin) == LOW) {
            
            //The rewind button on the remote has to be held down for manualAbort to take effect.

            PORTB = B00101000; //The actuators are now open.

            delay(ignitionTime); //Wait before ignition.

            PORTB = B00101010; //The ignition signal is sent.

            delay(triggerTime - ignitionTime);

            break;
        }

        //After the test time is over, reset all pins to their original state by calling the manualAbort function.

        manualAbort();
        }
      }
    }
  }
