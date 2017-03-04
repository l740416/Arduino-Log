
#include <TimeLib.h> 
#include "Arduino.h"
#include "Log.h"

Log::Log() {
  memset( buf, 0, sizeof(buf) );
  idx = 0;
  for( int i = 0; i < N_LOG_LOOP; i++ ) {
    timestamp_enter[i] = 0;
    timestamp_duration[i] = 0;
  }
}


size_t Log::write(uint8_t v) {
  memcpy( buf+idx, &v, 1 );
  idx += 1;
  if( idx >= LOG_SIZE ) idx = 0;
  Serial.write( v );
  return 1;  
}

size_t Log::write(const char *str) {
  return write( ( const uint8_t * ) str, strlen(str) );
}

size_t Log::write(const uint8_t *buffer, size_t size) {

  if( size >= 5 && buffer[0] == '[' && buffer[1] == 'M' && buffer[2] == 'S' && buffer[3] == 'G' && buffer[4] == ']' ) {
    char str[100];
    snprintf( str, 100, "[%02d:%02d:%02d]", hour(), minute(), second() );
    write( str );
    write( buffer + 5, size - 5 );
    return size - 5 + strlen(str);
  } 

  if( size >= LOG_SIZE ) return 0;
  if( idx + size >= LOG_SIZE ) {
    buf[idx] = '\0';
    idx = 0;
  }
  memcpy( buf + idx, buffer, size );
  idx += size;  

  Serial.write( buffer, size );

  
  /*
  if( idx + size >= LOG_SIZE ) {
    size_t n = LOG_SIZE - idx;
    memcpy( buf + idx, buffer, n );
    idx = size - n;
    memcpy( buf, buffer + n, idx );
    
  } else {
    memcpy( buf+idx, buffer, size );
    idx += size;
  }
  */
  return size;  
}

size_t Log::printf( char *fmt, ... ) {
  // resulting string limited to 256 chars
  char buf[256]; 
  va_list args;
  va_start( args, fmt );
  vsnprintf( buf, 256, fmt, args );
  va_end( args );
  return write( ( const uint8_t * ) buf, strlen(buf) );
}

void Log::show_status() {
  printf( "[MSG]  Serial loop: %dus\n", duration(LOG_SERIAL_LOOP) );
  printf( "[MSG]  Sensor loop: %dus\n", duration(LOG_SENSOR_LOOP) );
  printf( "[MSG] Nodenet loop: %dus\n", duration(LOG_NODENET_LOOP) );
  printf( "[MSG]    Wifi loop: %dus\n", duration(LOG_WIFI_LOOP) );
  printf( "[MSG]    Http loop: %dus\n", duration(LOG_HTTP_LOOP) );
  printf( "[MSG]     IoT loop: %dus\n", duration(LOG_IOT_LOOP) );
}

