/*
  Uptime.cpp - Uptime Library - implementation
*/

// include this library's description file
#include "Uptime.h"

// Constructors /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances
Uptime::Uptime(void) {
  // initialize this instance's variables
  Days = 0;
  Hours = 0;
  Minutes = 0;
  Seconds = 0;
  Milliseconds = 0;
  TotalSeconds = 0;
  lastMillis = 0;
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries

void Uptime::compute(void) {
  long now = millis();
  long diff = (now - lastMillis);
  lastMillis = now;
  Milliseconds += diff;
  if (Milliseconds > 1000) {
    TotalSeconds += (Milliseconds / 1000);
    Seconds += (Milliseconds / 1000);
    Milliseconds = Milliseconds % 1000;
    if (Seconds >= 60) {
      Minutes += (Seconds /  60);
      Seconds = Seconds % 60;                
      if (Minutes >= 60) {
        Hours += (Minutes / 60);
        Minutes = Minutes % 60;
        if (Hours >= 24) {
          Days += (Hours / 24);
          Hours = Hours % 24;
        }
      }
    }
  }
}

int Uptime::days(void) {
  return Days;  
}

int Uptime::hours(void) {
  return Hours;
}

int Uptime::minutes(void) {
  return Minutes;
}

int Uptime::seconds(void) {
  return Seconds;
} 

int Uptime::milliseconds(void) {
  return Milliseconds;
} 

long Uptime::totalSeconds(void) {
  return TotalSeconds;
} 


void Uptime::print(void) {
  // A função "F" economiza SRam
  Serial.print(F("Uptime: "));
  Serial.print(days());
  Serial.print(F(" Days  "));
  Serial.print(hours());
  Serial.print(F(" Hours  "));
  Serial.print(minutes());
  Serial.print(F(" Minutes  "));
  Serial.print(seconds());
  Serial.print(F(" Seconds  "));	
  Serial.print(milliseconds());
  Serial.print(F(" Milliseconds. "));	
  Serial.print(totalSeconds());
  Serial.println(F(" total seconds up"));	
}

// Private Methods /////////////////////////////////////////////////////////////