


#ifndef _LOG_H_
#define _LOG_H_

#define LOG_SIZE 1024

class Log : public Print 
{

  public:

    Log();
  
    virtual size_t write( uint8_t v );
    virtual size_t write( const char *str );
    virtual size_t write( const uint8_t *buffer, size_t size );
    size_t printf( char const *fmt, ... );

#ifdef F // check to see if F() macro is available
		size_t printf(const __FlashStringHelper *format, ...);
#endif

    const char * get() { return buf; }
    const char * get_first() { return buf+idx+1; }
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
};


#endif
