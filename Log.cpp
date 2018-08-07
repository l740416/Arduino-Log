
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
      snprintf( str, 64, "[%02d%02d%02d]", m_pHourFunc(), m_pMinuteFunc(), m_pSecondFunc() );
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
  char buf[LOG_MAX_PRINTF_BUF_SIZE]; 
  va_list args;
  va_start( args, fmt );
  vsnprintf( buf, sizeof(buf), fmt, args );
  va_end( args );
  return write( ( const uint8_t * ) buf, strlen(buf) );
}

#ifdef F // check to see if F() macro is available
size_t Log::printf(const __FlashStringHelper *fmt, ...)
{
  char buf[LOG_MAX_PRINTF_BUF_SIZE]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt);
  vsnprintf_P(buf, sizeof(buf), (const char *)fmt, args); // progmem for AVR
  va_end(args);
  return write( ( const uint8_t * ) buf, strlen(buf) );
}
#endif
