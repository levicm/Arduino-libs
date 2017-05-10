/*
  Uptime.cpp - Uptime Library - description
*/

// ensure this library description is only included once
#ifndef Uptime_h
#define Uptime_h

// include types & constants of Wiring core API
#if defined(ARDUINO) && ARDUINO < 100
#include "WProgram.h"
#else
#include "Arduino.h"
#endif

// library interface description
class Uptime
{
  // user-accessible "public" interface
  public:
    // constructors
    Uptime(void);
	// setters
	// getters
	long totalSeconds(void);

    int days(void);
    int hours(void);
	int minutes(void);
	int seconds(void);
	int milliseconds(void);

	// others
	void print(void);

  // library-accessible "private" interface
  private:
	int highMillis;
	int rollover;  
};

#endif