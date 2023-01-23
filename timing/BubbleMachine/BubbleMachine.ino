int greenLed = 13;
int yelLed = 12;
int redLed = 11;
int valvePin = 10; //Actuator signal pin.  State Low, i.e. pin bit = 0 == VALVE OPEN AND FLOW
int bubblePin = 9; //Ignition signal pin.
int CO2Pin    = 7; //N.O. solenoid for CO2 flow  State Low, i.e. pin bit = 0 == VALVE CLOSED AND NO FLOW

//Trigger remote pins

int forwardPin = 2;
int rewindPin = 4;
int trigger = 3;

bool checkGreen = LOW;
bool checkYel   = LOW;
bool checkRed   = LOW;

int buttonDelay  = 250; //Time you have to press a button on the remote for an action to ensue.
int triggerTime  = 2000; //Trigger length i.e. length of actuation of the valves.
int ignitionTime = 1000; //Ignition delay after the start of the trigger.
int CO2InjTime   = 500; //Duration CO2 is flushed to inert the sandworm environment

unsigned long startTime; //Time associated to the reading of the millis() function when the test starts.
unsigned long runTime; //Placeholder for the millis() function to use for comparison with trigger and ignition time.

void setup() {

  pinMode(greenLed,  OUTPUT);
  pinMode(yelLed,    OUTPUT);
  pinMode(redLed,    OUTPUT);
  pinMode(bubblePin, OUTPUT);
  pinMode(valvePin,  OUTPUT);
  pinMode(CO2Pin,    OUTPUT);

  pinMode(forwardPin, INPUT);
  pinMode(rewindPin,  INPUT);
  pinMode(trigger,    INPUT);

  PORTB = B00100100;
  digitalWrite(CO2Pin,HIGH);
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

void manualAbort() { //Disarms the system by resetting all control pins to their initial, safe state.

  digitalWrite(CO2Pin,HIGH);
  delay(CO2InjTime);
  digitalWrite(CO2Pin,LOW);
  
  PORTB = B00100100;

  checkRed = LOW;
  checkYel = LOW;
  checkGreen = LOW;
}

void loop() {

  PORTB = B00100100;
  digitalWrite(CO2Pin,HIGH);
  delay(1000);
  digitalWrite(CO2Pin,LOW);
  while (digitalRead(trigger) == HIGH) { //Timing Arduino can't advance if the trigger is on.

    unexpectedTrigger();
  }

  if (digitalRead(forwardPin) == HIGH) {

    checkGreen = HIGH;
    delay(buttonDelay);
  }

  while (checkGreen == HIGH) {

    PORTB = B00010100;

    if (digitalRead(trigger) == HIGH) {

      unexpectedTrigger();
    }

    if (digitalRead(forwardPin) == HIGH) {

      checkYel = HIGH;
      delay(buttonDelay);
    }
    
    if (digitalRead(rewindPin) == HIGH) {

      checkGreen = LOW;
      delay(buttonDelay);
    }

    while (checkYel == HIGH) {

      PORTB = B00001100;

      if (digitalRead(trigger) == HIGH) {

        checkRed = HIGH;
        delay(buttonDelay);

        //Define the start time and the initial value of the run time.
        
        startTime = millis(); 
        runTime = millis();
      }
      
      if (digitalRead(rewindPin) == HIGH) {

        checkYel = LOW;
        delay(buttonDelay);
      }

      while (checkRed == HIGH) {

        while (runTime - startTime < ignitionTime) {

          PORTB = B00101000; //The actuators are now open.
          
          if (digitalRead(rewindPin) == HIGH) {

            manualAbort();
            break;
          }

          runTime = millis();
        }

        while (runTime - startTime <= triggerTime) {

          PORTB = B00101010; //The ignition signal is sent.
          
          if (digitalRead(rewindPin) == HIGH) {

            manualAbort();
            break;
          }

          runTime = millis();
        }

        //After the test time is over, reset all pins to their original state by calling the manualAbort function.

        manualAbort();
      }
    }
  }
}
