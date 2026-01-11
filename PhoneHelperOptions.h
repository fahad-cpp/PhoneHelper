#ifndef PHONEHELPEROPTIONS
#define PHONEHELPEROPTIONS
#include <vector>
#include <string>

struct PhoneHelperOptions{
    bool forceWireless=false;
    bool forceUSB = false;
    bool verbose = false;
    std::vector<std::string> scrcpyOptions;
};
#endif