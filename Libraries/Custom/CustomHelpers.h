#ifndef CustomHelpers_h
#define CustomHelpers_h

#include <Arduino.h>

#include <CustomLogger.h>


class CustomHelpers {
    private:
        CustomLogger logger_;

    public:
        CustomHelpers(bool debug);
        int convert_hexstring_to_int(byte* command);
        int validate_hex_string(byte* hex);
};

#endif
