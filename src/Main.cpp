#include "PhoneHelper.h"
std::vector<std::string> getCmdOptions(int argc,char* argv[]){
    std::vector<std::string> cmdoptions; 
    if(argc > 1)cmdoptions.reserve(argc-1);
    for(int i=1;i<argc;i++){
        cmdoptions.push_back(std::string(argv[i]));
    }
    return cmdoptions;
}
int main(int argc,char* argv[]){
    LOG_DEBUG("DEBUG MODE:\n");
    std::vector<std::string> cmdoptions = getCmdOptions(argc,argv);
    PhoneHelper phoneHelper(cmdoptions);
    phoneHelper.start();
}