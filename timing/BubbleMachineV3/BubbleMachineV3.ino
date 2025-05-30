//#### WARNING ###//
// THIS CODE IS MEANT FOR USE WITH ARDUINO UNO R3 EXCLUSIVELY
// PORT MANIPULATION USED IN THIS CODE IS ONLY COMPATIBLE WITH AVR BASED ARDUINO BOARDS.
// THIS CODE IS INCOMPATIBLE WITH ARUIDNO R4 MINIMA.

int greenLed   = 13;
int yelLed     = 12;
int redLed     = 11;
int valvePin   = 10; //Actuator signal pin.  State Low, i.e. pin bit = 0 == VALVE OPEN AND FLOW
int bubblePin  = 9; //Ignition signal pin.
int CO2Pin     = 8; //N.O. solenoid for CO2 flow  State Low, i.e. pin bit = 0 == VALVE CLOSED AND NO FLOW
int N2PurgePin = 7; //N.C. solenoid for N2 purge after a shot.
int ScopeTrig  = 6; //This pin used only to trigger oscilloscopes

//Trigger remote pins

int forwardPin = 2;
int rewindPin  = 4;
int trigger    = 3;

bool checkGreen = LOW;
bool checkYel   = LOW;
bool checkRed   = LOW;

int buttonDelay  = 250; //Time you have to press a button on the remote for an action to ensue.
int noiseSuppressionDelay = 500; //Duration to wait before to check rewind state because of igniter noise


int triggerTime  = 900; //Trigger length i.e. length of actuation of the valves.
int ignitionTime = 240; //Ignition delay after the start of the trigger.
int CO2InjTime   = 10000; //Duration CO2 is flushed to inert the sandworm environment
int N2PurgeTime  = 2000; //Duration N2 is flushed through the engine after a hot fire
//int ignitionDuration = 20; //Pulse length for ignition signal. (not used yet)

unsigned long startTime; //Time associated to the reading of the millis() function when the test starts.
unsigned long runTime; //Placeholder for the millis() function to use for comparison with trigger and ignition time.

//PORT MANIPULAITON//
//In this code, PORTB and PORTD are controlled using direct port manipulation instead of using digitalWrite().
//Each digital pin is represented by a single 0 or 1 in an 8 bit number. See details below.
//PORTB => digital pins 8-13. Corresponding bits: PORTB = [X X 13 12 11 10 9 8]
//PORTD => digital pins 0-7. Corresponding bits: PORTD = [7 6 5 4 3 2 1 0]

//#### WARNING ####//
//The relay board used to control the solenoid valves is counterintuitive. 
//A LOW signal from a digital pin will close the circuit.
//Entirely cutting power to the board will open all AC circuits.
//This is why the HIGH and LOW assignments of pins 7, 8 and 10 appear to be reversed.


void setup() {

  pinMode(greenLed,  OUTPUT);
  pinMode(yelLed,    OUTPUT);
  pinMode(redLed,    OUTPUT);
  pinMode(bubblePin, OUTPUT);
  pinMode(valvePin,  OUTPUT);
  pinMode(CO2Pin,    OUTPUT);
  pinMode(N2PurgePin,OUTPUT);

  pinMode(forwardPin, INPUT);
  pinMode(rewindPin,  INPUT);
  pinMode(trigger,    INPUT);

  //N2PurgePin = LOW;
  PORTD = B10000000;
  PORTB = B00100100;
}

//LED colour code
//Green: Safe-initial step
//Yellow: CO2 flush of exhaust
//Red: CO2 flush of exhaust + N2 purge of engine.
//Red + Green: Firing
//Yellow + Green: N2 purge of engine for 2 seconds, then 10 seconds of CO2 flushingn exhaust.
//Yellow + Red: Unexpected trigger activation. Firing system frozen until trigger switched off.

void unexpectedTrigger() { //Freezes the timing Arduino until the trigger is switched off.

  PORTB = B00011100;
}

void manualAbort() { //Disarms the system by resetting all control pins to their initial, safe state.
  PORTB = B00110101; //Close Actuators 
  //
  PORTD = B00000001; //Begin N2 Purge
  delay(N2PurgeTime); //Purge for N2PurgeTime
  PORTD = B10000001; //Stop N2 Purge
  PORTB = B00110101; //Begin CO2 Flush
  delay(CO2InjTime); //Flush CO2 for CO2InjTime
  PORTB = B00100100; //Stop CO2 flush

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
    delay(buttonDelay);
  }

  while (checkGreen == HIGH) {

    PORTB = B00010101; //CO2 flush of exhaust begins.
    PORTD = B10000001; //Revert to N2 closed in case of back button

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

      PORTB = B00001101; //CO2 now flowing before trigger is flipped
      PORTD = B00000001; //N2 flowing before trigger is flipped

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

          PORTD = B10000001; //N2 now off
          PORTB = B00101001; //The actuators are now open.
          //PORTB = B00101000; //The actuators and CO2 are now open.
          
          if (digitalRead(rewindPin) == HIGH) {

            manualAbort();
            break;
          }

          runTime = millis();
        }

        while (runTime - startTime <= triggerTime) {

          PORTB = B00101011; //The ignition signal is sent.
          //PORTB = B00101010; //The ignition signal is sent.
          PORTD = B11000000; //Scope trigger signal is sent
          delay(noiseSuppressionDelay);
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
