#include "PhoneHelper.h"
#include <iostream>
#include <algorithm>
#include <thread>
void PhoneHelper::start(){
    //Look for a device
    system("adb kill-server >nul 2>nul");
    int deviceCount = getDevices();
    while(deviceCount<=0){
        LOG_INFO("Connect a device , turn on USB Debugging , and press enter to try again\n");
        std::cin.get();
        deviceCount = getDevices();
    }
    //Enable TCP/IP
    system("adb tcpip 5555 >nul 2>nul");

    //Find WLAN ip address
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    std::string ip = getIp();
    while(ip.length() == 0){
        LOG_INFO("Device not in the same network\n");
        LOG_INFO("Connect the device to your network , and press enter to try again\n");
        std::cin.get();
        ip = getIp();
    }
    std::string command = "adb connect " + ip;
    std::string output = runCommand(command.c_str());
    if(output.find("connected to") == std::string::npos){
        LOG_INFO("Failed to connect to the IP address:"<<ip<<"\n");
        return;
    }
    LOG_INFO("Successfully connected to "<<ip<<"\n");

    system("scrcpy --tcpip -e -m1920 >nul 2>nul");
    
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
int PhoneHelper::getDevices(){
    std::string devices = runCommand("adb devices 2>nul");
    int count = std::count(devices.begin(),devices.end(),'\n');
    count -= 2;
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