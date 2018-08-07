


#ifndef _LOG_H_
#define _LOG_H_

#define LOG_SIZE 1024
#define LOG_MAX_PRINTF_BUF_SIZE 1024

typedef int (*LOG_TIME_FUNC) ();

class Log : public Print 
{

  public:

    Log();
    Log(LOG_TIME_FUNC hour_func, LOG_TIME_FUNC minute_func, LOG_TIME_FUNC second_func);
  
    virtual size_t write( uint8_t v );
    virtual size_t write( const char *str );
    virtual size_t write( const uint8_t *buffer, size_t size );
    size_t printf( char const *fmt, ... );

#ifdef F // check to see if F() macro is available
		size_t printf(const __FlashStringHelper *format, ...);
#endif

    const char * get() { return buf; }
    const char * get_first() { return buf+idx+1; }
    const char * get_first_newline() { 
    	char* buf_ptr = buf+idx+1;
    	while(buf_ptr[0] != '\0' && buf_ptr[0] != '\n') { buf_ptr++; }
    	return buf_ptr+1; 
    }
    const char * get_second() { return buf; }

    void enter( int id ) {
      funcId = id;
    }
    
    int current_id() {
      return funcId;
    }   

    
  private:
    char buf[LOG_SIZE+1];
    size_t idx;

		int funcId;  
		
		LOG_TIME_FUNC m_pHourFunc;
		LOG_TIME_FUNC m_pMinuteFunc;
		LOG_TIME_FUNC m_pSecondFunc;
		
		bool m_overridesHeading; //<<< replace [MSG] heading with current time
};


#endif
