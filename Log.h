


#ifndef _LOG_H_
#define _LOG_H_

#define LOG_SIZE 4096

class Log : public Print 
{

  public:

    enum LOG_LOOP_TYPE {
      LOG_SERIAL_LOOP = 0,
      LOG_SENSOR_LOOP,
      LOG_NODENET_LOOP,
      LOG_WIFI_LOOP,
      LOG_HTTP_LOOP,
      LOG_IOT_LOOP,
      N_LOG_LOOP
    };
  
    Log();
  
    virtual size_t write( uint8_t v );
    virtual size_t write( const char *str );
    virtual size_t write( const uint8_t *buffer, size_t size );
    size_t printf( char *fmt, ... );

    const char * get() { return buf; }
    const char * get_first() { return buf+idx+1; }
    const char * get_second() { return buf; }

    void enter( enum LOG_LOOP_TYPE type ) {
      timestamp_enter[ type ] = micros();
    }
    
    void leave( enum LOG_LOOP_TYPE type ) {
      timestamp_duration[ type ] = micros() - timestamp_enter[ type ];
    }   
    
    long duration( enum LOG_LOOP_TYPE type ) {
      return timestamp_duration[ type ];
    }

    void show_status();
    
  private:
    char buf[LOG_SIZE];
    size_t idx;
    long timestamp_enter[ N_LOG_LOOP ];
    long timestamp_duration[ N_LOG_LOOP ];
  
};


#endif
