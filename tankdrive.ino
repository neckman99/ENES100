#include <SoftwareSerial.h>
#include "enes100.h"
#include <dfr_tank.h>

DFRTank tank;

SoftwareSerial mySerial(8, 9); //RX is pin 8, TX is pin 9
enes100::RfClient<SoftwareSerial> rf(&mySerial); 
enes100::Marker marker;
int markerNumber=107; //update this with whatever black and white marker you receive

void setup()
{
  mySerial.begin(9600); //this establishes serial communication with
                        //something other than serial monitor, in this
                        //case RF communication with mission control
  Serial.begin(9600); //this establishes regular serial communication
                      //through USB to student's serial monitor

  pinMode(8, INPUT); //since pin 8 is RX, it receives as an input
  pinMode(9, OUTPUT); //since pin 9 is TX, it transmits as an output
  
  rf.resetServer();
  rf.sendMessage("\nWetWare is Connected\n"); //sent to mission control
  Serial.println("Wetware is Connected"); //sent to student's serial monitor
  delay(1000);
  
  // Figure out orientation
  float dx = 3.100 - marker.x; float dy = .4 - marker.y; //Figure out distances from position
  float thetareq = atan(dx/dy); //Figure out required angle
  float dtheta = thetareq - marker.theta;
  tank.init();
  rf.sendMessage(marker.theta); rf.sendMessage(thetareq); //Tell RF the current angle and the needed angle.
  
  while (dtheta < -.1745 || dtheta > .1745) {
    tank.setLeftMotorPWM(-150); tank.setRightMotorPWM(150);
    delay(100); //Change this later? Only turns for .1 sec. 
    tank.turnOffMotors();
    float dtheta = thetareq - marker.theta;
  }
}
void loop()
  {
  float dx = 3.100 - marker.x; float dy = 3.100 - marker.y; //Figure out distances from position
  float distance = sqrt(sq(dx) + sq(dy)); //Gets distance from target.
  if (distance < .07) { //If we have arrived...
    rf.sendMessage("Splashdown!"); //Say so
    delay(81600000); //Wait for a long time
  }
  tank.setLeftMotorPWM(200); tank.setRightMotorPWM(200); //Go west, young man
  delay(1000);
  dx = 3.100 - marker.x; dy = .4 - marker.y; //Figure out distances from position
  float thetareq = atan(dx/dy); //Figure out required angle
  float dtheta = thetareq - marker.theta;
  while (dtheta < -.1745 || dtheta > .1745) {
    tank.setLeftMotorPWM(-150); tank.setRightMotorPWM(150);
    delay(100); //Change this later? Only turns for .1 sec. 
    tank.turnOffMotors();
    dtheta = thetareq - marker.theta;
  }
  }
