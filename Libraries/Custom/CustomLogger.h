#ifndef CustomLogger_h
#define CustomLogger_h

#include <Arduino.h>


class CustomLogger {
    private:
        bool enabled_;

    public:
        CustomLogger();
        CustomLogger(bool enabled);
        
        void debug(const char* msg);
        void debugln(const char* msg);
        void debug(const char** msg);
};

#endif
