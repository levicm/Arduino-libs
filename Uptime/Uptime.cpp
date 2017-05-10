/*
  Uptime.cpp - Uptime Library - implementation
*/

// include this library's description file
#include "Uptime.h"

// Constructors /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances
Uptime::Uptime(void) {
  // initialize this instance's variables
  highMillis = 0;
  rollover = 0;  
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries

long Uptime::totalSeconds(void) {
  // Verifica se está próximo de reiniciar o valor de millis()
  if (millis() >= 3000000000) {
    highMillis = 1;
  }
  // Se reiniciou o valor de millis, contabiliza
  if (millis() <= 100000 && highMillis == 1) {
    rollover++;
    highMillis=0;
  }
  return millis() / 1000;
}

int Uptime::milliseconds(void) {
  return millis() % 1000;
} 

int Uptime::seconds(void) {
  return totalSeconds() % 60;
} 

int Uptime::minutes(void) {
  return (totalSeconds() / 60) % 60;
}

int Uptime::hours(void) {
  return (totalSeconds() / (60 * 60)) % 24;
}

int Uptime::days(void) {
  //Parte inicial cuida da rolagem de dias (por volta de 50 dias)
  return (rollover * 50) + (totalSeconds() / (60 * 60 * 24));  
}

void Uptime::print(void) {
  // A função "F" economiza SRam
  Serial.print(F("Uptime:  "));
  Serial.print(days());
  Serial.print(F(" Days  "));
  Serial.print(hours());
  Serial.print(F(" Hours  "));
  Serial.print(minutes());
  Serial.print(F(" Minutes  "));
  Serial.print(seconds());
  Serial.print(F(" Seconds.  "));	
  Serial.print(totalSeconds());
  Serial.println(F(" total seconds up"));	
}
// Private Methods /////////////////////////////////////////////////////////////

