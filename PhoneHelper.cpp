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
    if(deviceCount >= 2)
        system("adb kill-server >nul 2>nul");
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
    if(!options.forceWireless && !options.forceUSB){
        LOG_INFO("Device not on the same network, connecting via USB\n"
            "Use PhoneHelper with --wireless option to keep waiting until connection\n"
            "Or try again with same network connection\n");
    }
    if((!options.forceWireless && (ip.length() == 0)) || options.forceUSB){
        LOG_INFO("scrcpy started\n");
        system("scrcpy -d -m1920 >nul 2>nul");
        LOG_INFO("scrcpy exited\n");
        return;
    }
    while(ip.length() == 0){
        LOG_INFO("Device not in the same network\n");
        LOG_INFO("Connect the device to your network , and press enter to try again\n");
        std::cin.get();
        ip = getIp();
    }

    // Connect to the device wirelessly
    std::string command = "adb connect " + ip;
    std::string output = runCommand(command.c_str());
    if(output.find("connected to") == std::string::npos){
        LOG_INFO("Failed to connect to the device with IP address:"<<ip<<"\n");
        return;
    }
    LOG_INFO("Successfully connected to "<<ip<<"\n");
    LOG_INFO("You can now safely remove the USB\n");

    LOG_INFO("scrcpy started\n");
    system("scrcpy --tcpip -e -m1920 >nul 2>nul");
    LOG_INFO("scrcpy exited\n");
    
}
std::string PhoneHelper::getIp(){
    std::string ipline = runCommand("adb shell ip addr show wlan0 2>nul | find \"inet \"");
    if(ipline.length() == 0){
        return "";
    }
    size_t length = ipline.length();
    size_t ipstart = ipline.find("inet ") + 5;
    size_t ipend = ipline.find('/');
    std::string ip = ipline.substr(ipstart,(ipend - ipstart));
    LOG_INFO("IP found:"<<ip<<"\n");
    
    return ip;
}
int PhoneHelper::getDeviceCount(bool silent){
    std::string devices = runCommand("adb devices 2>nul");
    int count = std::count(devices.begin(),devices.end(),'\n');
    count -= 2;

    //exit without logging
    if(silent){
        return count;
    }

    if(count > 1){
        LOG_INFO(count<<" devices are connected\n");
    }else if(count == 1){
        LOG_INFO(count<<" device is connected\n");
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