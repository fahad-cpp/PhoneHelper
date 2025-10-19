#ifndef PHONEHELPER
#define PHONEHELPER
#include "Logging.h"
#include <string>
class PhoneHelper{
    public:
    std::string runCommand(const std::string& command);
    void start();
    int getDevices();
    std::string getIp();
};
#endif