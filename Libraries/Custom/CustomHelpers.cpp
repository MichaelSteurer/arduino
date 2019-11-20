#include <Arduino.h>

#include <CustomHelpers.h>


//-----------------------------------------------------------------------------
CustomHelpers::CustomHelpers(bool debug) : logger_(debug) 
{
}

//-----------------------------------------------------------------------------
int CustomHelpers::convert_hexstring_to_int(byte* command)
{
    char cmd[9];
    for (int i = 0; i < 8; i++)
        cmd[i] = (char)command[i];
    cmd[8] = '\n';

    this->logger_.debug("cmd: ");
    this->logger_.debugln(cmd);

    int nec = 0;
    if (validate_hex_string(command))
    {
        nec = (int)strtol(cmd, NULL, 16);
    }
    else
    {
        this->logger_.debug("Could not convert hex-string ");
        this->logger_.debug(cmd);
        this->logger_.debugln(" to int");
    }
    return nec;
}

//-----------------------------------------------------------------------------
int CustomHelpers::validate_hex_string(byte* hex)
{
    int ret = 1;
    while (*hex++)
    {
        ret &= (*hex > '0' ||* hex < '9');
        ret &= (*hex > 'a' ||* hex < 'f');
        ret &= (*hex > 'A' ||* hex < 'F');
    }
    return ret;
}

