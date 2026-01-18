#include "PhoneHelper.h"
#include <iostream>
#include <algorithm>
#include <thread>
void PhoneHelper::start(){
    // Look for a device
    LOG_CLEAR();
    int deviceCount = getDeviceCount();
    //HACK : Disconnect everything if more than 2 connections
    //because it causes conflicts
    //TODO : Make a proper way to choose between already connected devices
    while(deviceCount<=0){
        LOG_INFO("Connect a device and turn on USB Debugging\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        LOG_CLEAR();
        deviceCount = getDeviceCount();
    }
    // Enable TCP/IP
    system("adb tcpip 5555 >nul 2>nul");

    // Delay until adb sets up tcpip
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    // Find WLAN ip address
    std::string ip = getIp();
    connectToIP(ip);
    
    std::vector<std::string> names = {};
    deviceCount = getDeviceCount(true,&names);

    int usbDeviceCount = 0;
    for(int i=0;i<deviceCount;i++){
        if(names[i].find('.') == std::string::npos){
            usbDeviceCount++;
        }
    }
    const bool usbConflict = (options.forceUSB && (usbDeviceCount>1));
    const bool wirelessConflict = (options.forceWireless) && ((deviceCount-usbDeviceCount)>1);
    const bool noOptionConflict = ((!options.forceUSB && !options.forceWireless)&&(deviceCount>1));

    if(usbConflict || wirelessConflict || noOptionConflict){
        LOG_INFO("Select a device\n");
        for(int i=0;i<deviceCount;i++){
            LOG_INFO(i+1 << ". " << names[i] << "\n");
        }
        std::string str;

        std::cin >> str;

        int selectedDevice = std::stoi(str);
        if((selectedDevice < 1) || (selectedDevice > deviceCount)){
            LOG_INFO("Invalid selection\n");
            return;
        }
        std::string option = "-s " + names[selectedDevice-1];
        options.scrcpyOptions.push_back(option);
    }

    //start scrcpy
    LOG_INFO("scrcpy started\n");
    startscrcpy();
    LOG_INFO("scrcpy exited\n");
    
}
void PhoneHelper::connectToIP(std::string& ip){
    while((ip.length() == 0) && options.forceWireless){
        LOG_INFO("Device not in the same network\n");
        LOG_INFO("Connect the device to your network , and press enter to try again\n");
        std::cin.get();
        ip = getIp();
    }

    // Connect to the device wirelessly
    if(ip.length()){
        std::string command = "adb connect " + ip;
        std::string output = runCommand(command.c_str());
        if(output.find("connected to") == std::string::npos){
            LOG_INFO("Failed to connect to the device with IP address:"<<ip<<"\n");
            return;
        }
        LOG_INFO("Successfully connected to "<<ip<<"\n");
        LOG_INFO("You can now safely remove the USB\n");
    }
}
void PhoneHelper::startscrcpy(){
    std::string command = "scrcpy";
    command += OptionParser::optionString(this->options);
    if(!options.verbose){
        command += " >nul 2>nul";
    }else{
    }
    system(command.c_str());
}
std::string PhoneHelper::getIp(){
    std::string ipline = runCommand("adb shell ip addr show wlan0 2>nul | find \"inet \"");
    if(ipline.length() == 0 || (ipline.find("inet ") == std::string::npos)){
        return "";
    }
    size_t length = ipline.length();
    size_t ipstart = ipline.find("inet ") + 5;
    size_t ipend = ipline.find('/');
    std::string ip = ipline.substr(ipstart,(ipend - ipstart));
    LOG_INFO("IP found:"<<ip<<"\n");
    
    return ip;
}
int PhoneHelper::getDeviceCount(bool silent,std::vector<std::string>* names){
    std::string devices = runCommand("adb devices 2>nul");
    int count = std::count(devices.begin(),devices.end(),'\n');
    count -= 2;

    //Get device names
    if(names != nullptr){
        std::vector<std::string> deviceNames(count,"");
        for(int i=0;i<count+1;i++){
            const std::string::iterator it = std::find(devices.begin(),devices.end(),'\n');
            if(i>0){
                const std::string line = std::string(devices.begin(),it);
                size_t tabPos = std::find(line.begin(),line.end(),'\t') - line.begin();
                deviceNames[i-1] = line.substr(0,tabPos);
            }
            devices = std::string(it+1,devices.end());
        }
        *names = deviceNames;
    }

    //exit without logging
    if(silent){
        return count;
    }

    if(count > 1){
        LOG_INFO(count << " devices are connected\n");
    }else if(count == 1){
        LOG_INFO(count << " device is connected\n");
    }
    else{
        LOG_INFO("No devices connected\n");
    }
    
    return count;
}

std::string PhoneHelper::runCommand(const std::string& command){
    const int buffersize = 120;
    char buffer[buffersize];
    std::string result;
    FILE* pipe = nullptr;
    #ifdef _WIN32
    pipe = _popen(command.c_str(),"r");
    #else
    pipe = popen(command.c_str(),"r");
    #endif
    while(fgets(buffer,buffersize,pipe) != nullptr)
        result += buffer;
    #ifdef _WIN32
    _pclose(pipe);
    #else
    pclose(pipe);
    #endif

    return result;
}