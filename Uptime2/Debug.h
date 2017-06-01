//#define DEBUG
#ifdef DEBUG
  #define DEBUG_PRINT(str) Serial.print(str);
  #define DEBUG_PRINTLN(str) Serial.println(str);
  #define DEBUG_PRINTLN2(str1, str2) Serial.print(str1); Serial.println(str2);
#endif

#ifndef DEBUG_PRINT
  #define DEBUG_PRINT(str)
#endif
#ifndef DEBUG_PRINTLN
  #define DEBUG_PRINTLN(str)
#endif
#ifndef DEBUG_PRINTLN2
  #define DEBUG_PRINTLN2(str1, str2)
#endif

