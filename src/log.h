#ifndef __LOG_H__
#define __LOG_H__

#include "Arduino.h"

// #define LOG(severity, msg) p msg;

// void p(const char *fmt, ... )
// {
//     static char tmp[128]; // resulting string limited to 128 chars.
//     va_list args;
//     va_start (args, fmt );
//     vsnprintf(tmp, 128, fmt, args);
//     va_end (args);
//     Serial.println(tmp);
//     delay(1);
// }

inline void LOG_SPACE_IMPL();
inline void LOG_SET_IMPL(int8_t i);
inline void LOG_SET_IMPL(int16_t i);
inline void LOG_SET_IMPL(int32_t i);
inline void LOG_SET_IMPL(int64_t i);
inline void LOG_SET_IMPL(uint8_t i);
inline void LOG_SET_IMPL(uint16_t i);
inline void LOG_SET_IMPL(uint32_t i);
inline void LOG_SET_IMPL(uint64_t i);
inline void LOG_SET_IMPL(char c);
inline void LOG_SET_IMPL(String s);
inline void LOG_SET_IMPL(float f);
inline void LOG_SET_IMPL(double d);
inline void LOG_NEW_LINE_IMPL();

#ifdef LOG_ENABLED

#define LOG(P) LOG_SET_IMPL(P)
#define LOG_NEW_LINE LOG_NEW_LINE_IMPL();
#define LOG_SPACE LOG_SPACE_IMPL()

#else

#define LOG(P)
#define LOG_NEW_LINE
#define LOG_SPACE

#endif


inline void LOG_INIT()
{
#if defined (LOG_ENABLED)
  Serial.begin(115200);
  LOG("Initialized: SERIAL_LOG64");
  LOG_NEW_LINE;
#endif

#if defined (EEPROM_LOG_ENABLED)
  LOG_EEPROM_READ;

  EEPROM_LOG_ADDR_PTR = EEPROM_LOG_Start_Address;
#endif
}

inline void LOG_SPACE_IMPL()
{
  Serial.print(" ");
}

inline void LOG_SET_IMPL(int8_t i)
{
  Serial.print(i);
  LOG_SPACE;
}

inline void LOG_SET_IMPL(int16_t i)
{
  Serial.print(i);
  LOG_SPACE;
}

inline void LOG_SET_IMPL(int32_t i)
{
  Serial.print(i);
  LOG_SPACE;
}

inline void LOG_SET_IMPL(uint8_t i)
{
  Serial.print(i);
  LOG_SPACE;
}

inline void LOG_SET_IMPL(uint16_t i)
{
  Serial.print(i);
  LOG_SPACE;
}

inline void LOG_SET_IMPL(uint32_t i)
{
  Serial.print(i);
  LOG_SPACE;
}


inline void LOG_SET_IMPL(char c)
{
  Serial.print(c);
  LOG_SPACE;
}


inline void LOG_SET_IMPL(String s)
{
  Serial.print(s);
  LOG_SPACE;
}

inline void LOG_SET_IMPL(float f)
{
  Serial.print(f);
  LOG_SPACE;
}

inline void LOG_SET_IMPL(double d)
{
  Serial.print(d);
  LOG_SPACE;
}

inline void LOG_NEW_LINE_IMPL()
{
  Serial.println(" ");
}

inline void LOG_SET_IMPL(uint64_t n)
{
  unsigned char buf[16 * sizeof(long)];

  if (n == 0)
  {
    Serial.print((char)'0');
    LOG_SPACE;
    return;
  }

  for (uint16_t i = 0; n > 0; i++)
  {
    buf[i] = n % 10;
    n /= 10;
  }

  for (uint16_t i = 0; i > 0; i--)
  {
    Serial.print((char) (buf[i - 1] < 10 ? ('0' + buf[i - 1]) : ('A' + buf[i - 1] - 10)));
  }

  LOG_SPACE;
}

inline void LOG_SET_IMPL(int64_t n)
{
  unsigned char buf[16 * sizeof(long)]; ;

  if (n == 0)
  {
    Serial.print((char)'0');
    LOG_SPACE;
    return;
  }

  if (n < 0)
  {
    Serial.print((char)'-');
    n = n * (-1);
  }

  for (uint16_t i = 0; n > 0; i++)
  {
    buf[i] = n % 10;
    n /= 10;
  }

  for (uint16_t i = 0; i > 0; i--)
  {
    Serial.print((char) (buf[i - 1] < 10 ? ('0' + buf[i - 1]) : ('A' + buf[i - 1] - 10)));
  }

  LOG_SPACE;
}


#endif // __LOG_H__