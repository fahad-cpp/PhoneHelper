#ifndef PHONEHELPER
#define PHONEHELPER
#include "Logging.h"
#include "OptionParser.h"

class PhoneHelper{
    PhoneHelperOptions options;
    public:
    PhoneHelper(const std::vector<std::string>& options){
        this->options = OptionParser::parse(options);
    }
    std::string runCommand(const std::string& command);
    void start();
    int getDeviceCount(bool silent = false,std::vector<std::string>* names = nullptr);
    void connectToIP(std::string& ip);
    void startscrcpy();
    std::string getIp();
};
#endif