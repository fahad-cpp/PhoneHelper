#ifndef LOGGING
#define LOGGING
#include <iostream>
#ifndef APPNAME
#define APPNAME "PhoneHelper"
#endif 
#define LOG_INFO(X) std::cout << "[" << APPNAME << "]:" << X 
#define LOG_WARN(X) std::cout << "[" << APPNAME << "]:[WARN]:" << X 
#ifdef NDEBUG
#define LOG_DEBUG(X)
#else
#define LOG_DEBUG(X) std::cout << "[DEBUG]:" << X 
#endif
#define LOG_CLEAR()\
system("cls");\
std::cout<<"\n";
#endif