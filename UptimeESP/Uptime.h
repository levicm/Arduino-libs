/**
 * Uptime.cpp - Uptime Library - description
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
    byte hours(void);
    byte minutes(void);
    byte seconds(void);
    int milliseconds(void);

	// others
	void begin(boolean persistent = false);
	void compute(void);
	void compute(long elapsedMillis);
	void persist(long sleepTimeInMillis = 0);
  void print(void);

  // library-accessible "private" interface
  private:
    long _totalSeconds;
    int _days;
    byte _hours;
    byte _minutes;
    byte _seconds;
    int _milliseconds;
    long _lastMillis;
};
#endif