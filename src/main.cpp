#include <Arduino.h>
// Program to demonstrate the MD_Parola library
//
// Uses the Arduino Print Class extension with various output types
//
// MD_MAX72XX library can be found at https://github.com/MajicDesigns/MD_MAX72XX
//

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

const uint16_t WAIT_TIME = 1000;

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 8
//#define NUM_ZONES   2

#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10

const char* time[6] = {""};
const char* mode[6] = {""};

int workSeconds = 20;
int restSeconds = 10;

int savedWorkSeconds;
int savedRestSeconds;

int interval = 3;
bool workMode = true;

// Hardware SPI connection
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// Arbitrary output pins
// MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

void printTimer(int timeToPrint){
  Serial.println("I am in time to print");
  String timeString = " ";

  //P.displayZoneText(0, time[0], PA_CENTER, 0, 0, PA_PRINT);

  if (timeToPrint > 0){
    timeString = String(timeToPrint / 60);
    timeString += ":";
    if (timeToPrint % 60 < 10){
        timeString += "0";
      }
    timeString += (String(timeToPrint % 60));
  }
  else{
    //timeString = "0:00";
    if(interval > 0){
      if (workMode) {
        workMode = false;
        interval--;
        restSeconds = savedRestSeconds;
      }
      else {
        workMode = true;
        workSeconds = savedWorkSeconds;
      }
    }
    else {
      timeString = "0:00";
    }
  }

  timeString.toCharArray(*time, 5);

  

  //while (P.displayAnimate() != true){}

  //P.print(timeString);

  Serial.println(timeString);

}

void printCurrentMode(){

  Serial.println("I am in print current mode");

  String modeString = "";

  //P.displayZoneText(1, mode[0], PA_CENTER, 0, 0, PA_PRINT);

  if(interval > 0){
    if (workMode){
      modeString = "Work";
    }
    else {
      modeString = "Rest";
    }
  }
  else {
    modeString = "Time";
  }

  modeString.toCharArray(*mode, 5);
  

  Serial.println(modeString);

  //while (P.displayAnimate() != true){}

}





//Timer function called every 1hz
ISR(TIMER1_COMPA_vect){
  P.displayReset(0);
  P.displayReset(1);
  if(workMode){
    printTimer(workSeconds);
    workSeconds--;
  }
  else {
    printTimer(restSeconds);
    restSeconds--;
  }
  printCurrentMode();
  P.displayAnimate();
  while(P.getZoneStatus(0)){}
  while(P.getZoneStatus(1)){}
  
}





void setup(void)
{
  Serial.begin(9600);
  //Timer stuff:
  cli();//stop interrupts

  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();//allow interrupts
  
  //display stuff:
  P.begin();

  P.setZone(0, 0, 3);
  P.setZone(1, 4, 7);
  

  P.displayZoneText(1, time[0], PA_CENTER, 0, 0, PA_PRINT);
  P.displayZoneText(0, mode[0], PA_CENTER, 0, 0, PA_PRINT);
  

  savedWorkSeconds = workSeconds;
  savedRestSeconds = restSeconds;

}

void loop(void)
{
  /*P.print("Hello");
  delay(WAIT_TIME);
  P.print(1234, DEC);
  delay(WAIT_TIME);
  P.print(1234, HEX);
  delay(WAIT_TIME);
  P.print(12.5);      // float not supported by Arduino Print class
  delay(WAIT_TIME);
  P.print(9876l);
  delay(WAIT_TIME);
  P.println("end");   // only get the /r/n characters - avoid using println
  delay(WAIT_TIME);
  P.write('A');
  delay(WAIT_TIME);
  P.write('B');
  delay(WAIT_TIME);
  P.write('C');
  delay(WAIT_TIME);*/
  delay(100);
  


}