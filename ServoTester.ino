/*
 * DEVELOPED by Tiago Santos (tmasantos.com)
 * -- This program is a tool that enables you to perform servo operations easily (centering, manual and swipe).
 * -- It was developed for the Attiny 85, but also works on any Arduino. Just change the "defines" defined bellow.
 */

#include "SoftwareServo.h"                    //SOURCE: http://playground.arduino.cc/ComponentLib/Servo

/*
 * Attiny 85 I/O
 */
#define LEDS 0                                // PWM Output
#define SERVO 1                               // PWM Output
#define BUTTON 2                              // Analog Input
#define POTENTIOMETER 3                       // Analog Input

SoftwareServo myservo;                        // create servo object to control a servo 

int angle= 0;                                 //Servo current angle
int b_state=1;                                //Saves button 
int mode= 0;                                  //Current mode 0: centering mode (90º); 1: potentiometer mode; 2: swipe mode.

/*
 * Swipe between 0 and 180
 */
void modeThree(){                
  if(++angle > 180) angle=0;                  // Swipe from 0º to 179º
  myservo.write(angle); 
}

/*
 * Potentiometer mode (Example from arduino website)
 */
void modeTwo(){
  int val = analogRead(POTENTIOMETER);        // reads the value of the potentiometer (value between 0 and 1023) 
  val = map(val, 0, 1023, 0, 179);            // scale it to use it with the servo (value between 0 and 180) 
  myservo.write(angle= val);                  // sets the servo position according to the scaled value 
}

/*
 * Put servo @ 90º
 */
void modeOne(){
  myservo.write(angle= 90); 
}

/*
 * Get current mode. If button is pressed, change mode.
 */
int getMode(){
  if(isChangeState()){
    mode++;
    if(mode > 2)
      mode = 0;
  }
  return mode;
}

/*
 * Check if button is pressed
 */
bool isChangeState(){
  char state= analogRead(BUTTON) > 512? 1 : 0;
  bool changed=  (b_state != state) && b_state == 1;

  //Debounce is at loop()! > 15 needed
  
  b_state= state;
  return changed;
}

void setup() 
{ 
  myservo.attach(SERVO);                        // attaches the servo on pin 2 to the servo object
  pinMode(LEDS, OUTPUT);                        // sets the pin as output
  pinMode(POTENTIOMETER, INPUT);                // sets the pin as input
  pinMode(BUTTON, INPUT);                       // sets the pin as input
} 

void loop() 
{ 
  //Get current mode and set the LEDs accordingly
  switch(getMode()){
    case 0: modeOne(); analogWrite(LEDS,127); break;
    case 1: modeTwo(); analogWrite(LEDS,0); break;
    case 2: modeThree(); analogWrite(LEDS,255); break;
  }

  delay(30);
  SoftwareServo::refresh();                     //Needed for Software servo fo work
} 


