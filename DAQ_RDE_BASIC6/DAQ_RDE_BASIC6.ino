//this code can sample all analog pins on a teensy 4.1 at 28kHz. The sampling rate will increase as the number of pins used lowers
// you can modify the number of pins by changing the global variabl PINS in line 17
// the data is saved to a csv file on the sd card and the built in LED will blink 10 times when the data is saved to the sd card
//happy sampling!! - ED


#include <time.h>
#include <ADC.h>
//#include <ADC_util.h>
#include <SD.h>
#include <digitalWriteFast.h>
#include <iostream>
#include <string>


ADC *adc = new ADC(); // adc object
elapsedMicros t;

//only change these two parameters
#define PINS 18
const int DAQtrigger = 9;

uint8_t adc_pins[] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17};     
int value = 0;
int pin = 0;


void setup()
{

    pinMode(LED_BUILTIN, OUTPUT);          //initializing pins
    pinMode(DAQtrigger, INPUT);

    for (int i = 0; i < PINS; i++)        //loop to set all analog pins as inputs
    {
        pinMode(adc_pins[i], INPUT);
    }

    Serial.begin(230400);
    while (!Serial) {;        
  
    // Wait for serial port to connect. Needed for native USB port only
 
    }
    Serial.println("Initializing SD card...");

    // see if the card is present and can be initialized:
  
    while (!SD.begin(BUILTIN_SDCARD))          
    //{
      Serial.println("Waiting for SD card...");
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
   //  }
    //Serial.println("Card initialized.");
    digitalWrite(LED_BUILTIN, HIGH);


    ///// ADC0 ////
    adc->adc0->setAveraging(0);                                    // set number of averages
    adc->adc0->setResolution(10);                                   // set bits of resolution
    adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_HIGH_SPEED); // change the conversion speed
    adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED);     // change the sampling speed

////// ADC1 /////
#ifdef ADC_DUAL_ADCS
    adc->adc1->setAveraging(0);                                    // set number of averages
    adc->adc1->setResolution(10);                                   // set bits of resolution
    adc->adc1->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_HIGH_SPEED); // change the conversion speed
    adc->adc1->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED);     // change the sampling speed
#endif
    
    t = 0; //set the time to 0 (will still be larger than 0 though) 
}

    
void loop()
{
 
  while (digitalReadFast(DAQtrigger) == 1)
  { 
    digitalWriteFast(LED_BUILTIN, LOW);                                     // signal that the teensy is recording (for debugging)
    String dataString = "";                                             // make a string for assembling the data to log:
    for (int i = 0; i < PINS; i++)                                  //for loop to cycle through all the pins
    {
      dataString += String(adc->analogRead(adc_pins[i]));
      dataString += ",";
      }
     dataString += String(t);
     Serial.println(dataString);
  }
  while (digitalReadFast(DAQtrigger) == 0)
  {
    digitalWriteFast(LED_BUILTIN, HIGH);                                 //signal that data has stopped being recorded (for debugging)

    File dataFile = SD.open("datalog.csv", FILE_WRITE); 
    dataFile.println(Serial);                                  //save the data to the file on the SD card
    dataFile.close();                                  //close the file after data has been recorded

    for (int e = 0; e < 11; e++)
   {
   digitalWriteFast(LED_BUILTIN, LOW);
   delay(100);
   digitalWriteFast(LED_BUILTIN, HIGH);
   }

    }

}
