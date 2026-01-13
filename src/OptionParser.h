#ifndef OPTIONPARSER
#define OPTIONPARSER
#include "PhoneHelperOptions.h"
#include "Logging.h"

namespace OptionParser{
    PhoneHelperOptions parse(const std::vector<std::string>& _options);
    std::string optionString(const PhoneHelperOptions& _options);
};
#endif