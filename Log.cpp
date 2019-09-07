
#include "Arduino.h"
#include <stdarg.h>
#include "Log.h"

Log::Log()
{
  memset( buf, '\0', sizeof(buf) );
  idx = 0;
  m_pHourFunc = NULL;
	m_pMinuteFunc = NULL;
	m_pSecondFunc = NULL;
	m_overridesHeading = false;
	
	m_lastHour = -1;
  m_lastMinute = -1;
  m_lastSecond = -1;
}

Log::Log(LOG_TIME_FUNC hour_func, LOG_TIME_FUNC minute_func, LOG_TIME_FUNC second_func)
{
  memset( buf, '\0', sizeof(buf) );
  idx = 0;
  m_pHourFunc = hour_func;
	m_pMinuteFunc = minute_func;
	m_pSecondFunc = second_func;
	
	if(m_pHourFunc != NULL && m_pMinuteFunc != NULL && m_pSecondFunc != NULL)
  {
    m_overridesHeading = true;
  }
}

size_t Log::write(uint8_t v)
{
	buf[idx] = v;
  idx += 1;
  buf[idx] = '\0';
  if( idx >= LOG_SIZE )
  {
  	idx = 0;
  	buf[0] = '\0';
  }
  Serial.write( v );
  return 1;  
}

size_t Log::write(const char *str) {
  return write( ( const uint8_t * ) str, strlen(str) );
}

size_t Log::write(const uint8_t *buffer, size_t size) {

  if(m_overridesHeading == true)
  {  
    if( size >= 5 && buffer[0] == '[' && buffer[1] == 'M' && buffer[2] == 'S' && buffer[3] == 'G' && buffer[4] == ']' ) {
      
      char str[64];
      if ((m_lastHour   == m_pHourFunc()) &&
          (m_lastMinute == m_pMinuteFunc()) &&
          (m_lastSecond == m_pSecondFunc()))
      {
        snprintf(str, 64, "\t");
      }
      else
      {
        m_lastHour = m_pHourFunc();
        m_lastMinute = m_pMinuteFunc();
        m_lastSecond = m_pSecondFunc();
        snprintf(str, 64, "[%02d%02d%02d]", m_lastHour, m_lastMinute, m_lastSecond);
      }
      
      write( str );
      write( buffer + 5, size - 5 );
      return size - 5 + strlen(str);
    } 
  }
  
  for(size_t i = 0; i < size; i++)
  {
  	write(buffer[i]);
  }

  return size;  
}

size_t Log::printf( char const *fmt, ... )
{
  va_list arg;
  va_start(arg, fmt);
  char temp[64];
  char* buffer = temp;
  size_t len = vsnprintf(temp, sizeof(temp), fmt, arg);
  va_end(arg);
  if (len > sizeof(temp) - 1)
  {
    buffer = new char[len + 1];
    if (!buffer)
    {
      return 0;
    }
    va_start(arg, fmt);
    vsnprintf(buffer, len + 1, fmt, arg);
    va_end(arg);
  }
  len = write((const uint8_t*) buffer, len);
  if (buffer != temp)
  {
      delete[] buffer;
  }
  return len;
}

#ifdef F // check to see if F() macro is available
size_t Log::printf(const __FlashStringHelper *fmt, ...)
{
  va_list arg;
  va_start(arg, fmt);
  char temp[64];
  char* buffer = temp;
  size_t len = vsnprintf_P(temp, sizeof(temp), (PGM_P)fmt, arg);
  va_end(arg);
  if (len > sizeof(temp) - 1)
  {
    buffer = new char[len + 1];
    if (!buffer)
    {
      return 0;
    }
    va_start(arg, fmt);
    vsnprintf_P(buffer, len + 1, (PGM_P)fmt, arg);
    va_end(arg);
  }
  len = write((const uint8_t*) buffer, len);
  if (buffer != temp)
  {
      delete[] buffer;
  }
  return len;
}
#endif
