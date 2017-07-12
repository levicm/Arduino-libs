/**
 * Uptime.cpp - Uptime Library - implementation
 *
 * ESP specific library with persistence support.
 * To use the persistence support, call begin(true) e call persist() 
 * before ESP.deepSleep(). 
 * If you like to account the sleep time, inform it calling persist(sleepTimeInMillis).
 *
 * Levi Mota - levi.mota@gmail.com | http://github.com/levicm
 */
// include this library's description file
#include "Uptime.h"
#include "Debug.h"

extern "C" {
  #include "user_interface.h"
}

// Constructors /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances
Uptime::Uptime(void) {
  // initialize this instance's variables
  _days = 0;
  _hours = 0;
  _minutes = 0;
  _seconds = 0;
  _milliseconds = 0;
  _totalSeconds = 0;
  _lastMillis = 0;
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries

int Uptime::days(void) {
  return _days;  
}

byte Uptime::hours(void) {
  return _hours;
}

byte Uptime::minutes(void) {
  return _minutes;
}

byte Uptime::seconds(void) {
  return _seconds;
} 

int Uptime::milliseconds(void) {
  return _milliseconds;
} 

long Uptime::totalSeconds(void) {
  return _totalSeconds;
} 

void Uptime::begin(boolean persistent) {
  DEBUG_PRINTLN2("[Uptime.begin].persistent: ", persistent);
  if (persistent && ESP.getResetReason() == "Deep-Sleep Wake") {
    DEBUG_PRINTLN("[Uptime.begin] backing from sleep");
    byte rtcStore[4];
    system_rtc_mem_read(64, rtcStore, 4);
    #if defined(DEBUG)
    for (int i = 0; i < 4; ++i) {
      Serial.println(rtcStore[i]);
    }
    #endif
    // Se os 2 primeiros bytes forem "UP"
    if ((char) rtcStore[0] == 'U' && (char) rtcStore[1] == 'P') {
      // Pega os 2 bytes seguintes e guarda em _milliseconds
      _milliseconds = (rtcStore[2] << 8) | rtcStore[3];
      // Pega os 4 bytes seguintes e guarda em _totalSeconds
      system_rtc_mem_read(68, rtcStore, 4);
      #if defined(DEBUG)
      for (int i = 0; i < 4; ++i) {
        Serial.println(rtcStore[i]);
      }
      #endif
      _totalSeconds = (rtcStore[0] << 24) |
                      (rtcStore[1] << 16) |
                      (rtcStore[2] << 8) |
                      (rtcStore[3]);
      _seconds = _totalSeconds % 60;
      _minutes = (_totalSeconds / 60) % 60;
      _hours = (_totalSeconds / (60 * 60)) % 24;
      _days = (_totalSeconds / (60 * 60 * 24));  

      system_rtc_mem_read(72, rtcStore, 4);
      #if defined(DEBUG)
      for (int i = 0; i < 4; ++i) {
          Serial.println(rtcStore[i]);
      }
      #endif
      long sleepTimeInMillis = (rtcStore[0] << 24) |
                                (rtcStore[1] << 16) |
                                (rtcStore[2] << 8) |
                                (rtcStore[3]);
      if (sleepTimeInMillis > 0) {
        compute(sleepTimeInMillis);
      }
    }
    #if defined(DEBUG)
    print();
    #endif
  }
  compute();
  #if defined(DEBUG)
  print();
  #endif
}

void Uptime::compute(void) {
  long now = millis();
  long diff = (now - _lastMillis);
  _lastMillis = now;
  compute(diff);
}

void Uptime::compute(long elapsedMillis) {
  _milliseconds += elapsedMillis;
  if (_milliseconds > 1000) {
    _totalSeconds += (_milliseconds / 1000);
    _seconds += (_milliseconds / 1000);
    _milliseconds = _milliseconds % 1000;
    if (_seconds >= 60) {
      _minutes += (_seconds /  60);
      _seconds = _seconds % 60;
      if (_minutes >= 60) {
        _hours += (_minutes / 60);
        _minutes = _minutes % 60;
        if (_hours >= 24) {
          _days += (_hours / 24);
          _hours = _hours % 24;
        }
      }
    }
  }
}

void Uptime::persist(long sleepTimeInMillis) {
  // The persistence model is:
  // [b0|b1|b2|b3|b4|b5|b6|b7]
  // b0 and b1 : characters "UP"
  // b2 and b3 : value of _millisecond  (2 bytes int)
  // b4 to b7  : value of _totalSeconds (4 bytes long) 
  compute();
  #if defined(DEBUG)
  print();
  #endif
  byte rtcStore[4];
  rtcStore[0] = (byte) 'U';
  rtcStore[1] = (byte) 'P';
  rtcStore[2] = (byte)((_milliseconds >> 8) & 0xFF); 
  rtcStore[3] = (byte)(_milliseconds & 0xFF);
  #if defined(DEBUG)
  for (int i = 0; i < 4; ++i) {
    Serial.println(rtcStore[i]);
  }
  #endif
  system_rtc_mem_write(64, rtcStore, 4);

  rtcStore[0] = (byte)((_totalSeconds >> 24) & 0xFF); 
  rtcStore[1] = (byte)((_totalSeconds >> 16) & 0xFF); 
  rtcStore[2] = (byte)((_totalSeconds >> 8) & 0xFF); 
  rtcStore[3] = (byte)(_totalSeconds & 0xFF);
  #if defined(DEBUG)
  for (int i = 0; i < 4; ++i) {
    Serial.println(rtcStore[i]);
  }
  #endif
  system_rtc_mem_write(68, rtcStore, 4);

  rtcStore[0] = (byte)((sleepTimeInMillis >> 24) & 0xFF); 
  rtcStore[1] = (byte)((sleepTimeInMillis >> 16) & 0xFF); 
  rtcStore[2] = (byte)((sleepTimeInMillis >> 8) & 0xFF); 
  rtcStore[3] = (byte)(sleepTimeInMillis & 0xFF);
  system_rtc_mem_write(72, rtcStore, 4);
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