int dataInput = A0;
int reset = 3;
int warningLight = 11;
int go = 12; //Defines all used ports.

int Pmax, sensor = 30; //Maximum desired blast chamber pressure and pressure reading from transducer, in psi.
double offset = 0.00; //Offset to read atmospheric, in V. Recalibrate for new transducer.

//Offset values
//667 psi transducer, offset = 0.0881559
//30 psi transducer, offset = ??

double rawData;
double voltage;
double psi;

void setup() {
  
  //Serial.begin(9600);
  pinMode(reset, INPUT);
  pinMode(warningLight, OUTPUT);
  pinMode(go, OUTPUT);
}

void loop() {
  
  digitalWrite(go, LOW);
  digitalWrite(warningLight, LOW);
  rawdata = analogRead(dataInput);
  voltage = 5 * rawData / 1023 + offset;
  psi = 0.25 * (voltage - 0.5) * sensor;
  //Serial.println(psi);
  //delay(500);
  if (psi > Pmax) {

    digitalWrite(go, HIGH);
    digitalWrite(warningLight, HIGH);
    //Serial.println("MAX CHAMBER PRESSURE EXCEEDED.");
    //Serial.println("CHAMBER MAY BE FILLED WITH UNBURNED FUEL. USE CAUTION TO VENT CHAMBER.");
    //Serial.println("PRESS RESET BUTTON WHEN CHAMBER IS SAFE AND DEPRESSURIZED.");
    while (digitalRead(reset) == LOW) {
      
    }
  }
}
