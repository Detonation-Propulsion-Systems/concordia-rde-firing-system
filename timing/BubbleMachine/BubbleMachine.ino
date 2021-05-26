int GreenLed=13;
int YelLed=12;
int RedLed=11;
int ValvePin=10;
int BubblePin=9; //Detonation signal pin

int ForwardPin=2;
int RewindPin=4;
int Trigger=3;

bool checkGreen=LOW;
bool checkYel=LOW;
bool checkRed=LOW;

unsigned long TimeIni=0;
unsigned long Time=0;
int looptime   = 500; //Trigger length
int ignition   = 100; //Ignition delay

void setup() {
  pinMode(GreenLed,OUTPUT);
  pinMode(YelLed,OUTPUT);
  pinMode(RedLed,OUTPUT);
  pinMode(BubblePin,OUTPUT);
  pinMode(ValvePin,OUTPUT);

  pinMode(ForwardPin,INPUT);
  pinMode(RewindPin,INPUT);
  pinMode(Trigger,INPUT);
}

//LED colour code:
//Green: Safe-Initial step
//Yellow: Intermediate step (also safe)
//Red: Intermediate step (also safe)
//Red+Green: Firing
//Yellow+Red: Unexpected Trigger activation. Firing system frozen until trigger switched off.

void loop() {
  digitalWrite(GreenLed,HIGH);
  digitalWrite(YelLed,LOW);
  digitalWrite(RedLed,LOW);
  digitalWrite(ValvePin,HIGH); //This is a low valve, meaning it opens when on low.
  digitalWrite(BubblePin,LOW);
  while(digitalRead(Trigger)==HIGH){//this while loop added to original code. Timing arduino can't advance if trigger is on
    digitalWrite(YelLed, HIGH);
    digitalWrite(RedLed, HIGH);
    digitalWrite(GreenLed, LOW);
     }
  if(digitalRead(ForwardPin)==HIGH){
    checkGreen=HIGH;
    delay(300);
   }

  while(checkGreen==HIGH){
    digitalWrite(GreenLed,LOW);
    digitalWrite(YelLed,HIGH);
    digitalWrite(RedLed,LOW);
    digitalWrite(ValvePin,HIGH);
    digitalWrite(BubblePin,LOW);
    if(digitalRead(Trigger)==HIGH){
      while(digitalRead(Trigger)==HIGH){
        digitalWrite(YelLed, HIGH);
        digitalWrite(RedLed, HIGH);
        digitalWrite(GreenLed, LOW);
      }
    }
    if(digitalRead(ForwardPin)==HIGH){
      checkYel=HIGH;
      delay(300);
    }
    if(digitalRead(RewindPin)==HIGH){
      checkGreen=LOW;
      delay(300);
    }
     while(checkYel==HIGH){
      digitalWrite(GreenLed,LOW);
      digitalWrite(YelLed,LOW);
      digitalWrite(RedLed,HIGH);
      digitalWrite(ValvePin,HIGH);
      digitalWrite(BubblePin,LOW);
      
      if(digitalRead(Trigger)==HIGH){ //was forwardpin
        checkRed=HIGH;
        delay(300);
      }
      
      if(digitalRead(RewindPin)==HIGH){
        checkYel=LOW;
        delay(300);
      }
      
      while(checkRed==HIGH){
        digitalWrite(GreenLed,HIGH);
        digitalWrite(YelLed,LOW);
        digitalWrite(RedLed,HIGH);
        digitalWrite(ValvePin,LOW); //The valve is now opened
        digitalWrite(BubblePin,LOW);

        TimeIni=millis(); //Checks what initial time is

        while(checkRed==HIGH){ // This loops waits before sending trigger signal to bubble machine.
          if(digitalRead(RewindPin)==HIGH){
            
            digitalWrite(GreenLed,HIGH);
            digitalWrite(YelLed,LOW);
            digitalWrite(RedLed,LOW);
            digitalWrite(ValvePin,HIGH); //Right away sets those values to the precedent to stop everything as fast as possible for safety
            digitalWrite(BubblePin,LOW);
            
            checkRed=LOW;
            checkYel=LOW;
            checkGreen=LOW;
            delay(300);
          }
          
          Time=millis()-100; //Loop lasts about a tenth a second (depending on computational time).
          
          if(Time>=TimeIni){
            break;
          }
        }


        TimeIni=millis(); //Resets initial time
        
        while(checkRed==HIGH){ // This loop sends the trigger signal to the bubble machine while the valve is still opened.
          delay(ignition);
          digitalWrite(GreenLed,HIGH);
          digitalWrite(YelLed,LOW);
          digitalWrite(RedLed,HIGH);
          digitalWrite(ValvePin,LOW);
          digitalWrite(BubblePin,HIGH); //The detonation signal is sent

          Time=millis()-100; //The signal is sent for maximum 100ms
          if(digitalRead(RewindPin)==HIGH){
            
            digitalWrite(GreenLed,HIGH);
            digitalWrite(YelLed,LOW);
            digitalWrite(RedLed,LOW);
            digitalWrite(ValvePin,HIGH); //Right away sets those values to the precedent to stop everything as fast as possible for safety
            digitalWrite(BubblePin,LOW);
            
            checkRed=LOW;
            checkYel=LOW;
            checkGreen=LOW;
            delay(300);
          }
          if(Time>=TimeIni){
            break;
          }
        }
        
        digitalWrite(BubblePin,LOW); //detonation signal is off.
        
        while(checkRed==HIGH){ // This loops wait before closing the valve.

          Time=millis()-looptime; //The entirety of the loop lasts maximum five seconds.
          
          if(Time>=TimeIni){
            checkGreen=LOW;
            checkYel=LOW;
            checkRed=LOW;
            break;
          }

          if(digitalRead(RewindPin)==HIGH){
            
            digitalWrite(GreenLed,HIGH);
            digitalWrite(YelLed,LOW);
            digitalWrite(RedLed,LOW);
            digitalWrite(ValvePin,HIGH); //Right away sets those values to the precedent to stop everything as fast as possible for safety
            digitalWrite(BubblePin,LOW);
            
            checkRed=LOW;
            checkYel=LOW;
            checkGreen=LOW;
            delay(300);
          }
        }
      }
    }
  }
}
