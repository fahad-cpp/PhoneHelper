#include "OptionParser.h"

PhoneHelperOptions OptionParser::parse(const std::vector<std::string>& _options){
        PhoneHelperOptions options;
        for(const std::string& option : _options){
            if(option == "--wireless"){
                LOG_INFO("DEBUG:forceWireless is set\n");
                options.forceWireless = true;
                if(options.forceUSB){
                    LOG_INFO("WARN:--wireless was specified after --usb , --usb is overwritten\n");
                    options.forceUSB = false;
                }
                continue;
            }else if(option == "--usb"){
                LOG_INFO("DEBUG:forceUSB is set\n");
                options.forceUSB = true;
                if(options.forceWireless){
                    LOG_INFO("WARN:--usb was specified after --wireless , --wireless is overwritten\n");
                    options.forceWireless = false;
                }
                continue;
            }else if(option == "--verbose" || option == "-v"){
                LOG_INFO("Verbose output");
                options.verbose = true;
                continue;
            }
            //TODO : check for valid scrcpy options
            options.scrcpyOptions.push_back(option);
        }
        if(options.forceWireless){
            options.scrcpyOptions.push_back("-e");
        }else if(options.forceUSB){
            options.scrcpyOptions.push_back("-d");
        }else{
            options.forceWireless = true; //HACK: enable forceWireless by default to fix multiple devices
        }

        return options;
    }
    std::string OptionParser::optionString(const PhoneHelperOptions& _options){
        std::string optStr;
        for(std::string opt : _options.scrcpyOptions){
            optStr += (" " + opt);
        }
        return optStr;
    }