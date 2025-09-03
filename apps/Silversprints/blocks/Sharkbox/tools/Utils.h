#pragma once

#include <iostream>
#include <sstream>

namespace sb { namespace utils {  

using namespace std;
    
inline std::string millisToTimestamp( int millis, int precision = 3 ){
	
    int ms = millis % 1000;
    int seconds = int(millis / 1000) % 60 ;
    int minutes = int(millis / (1000*60));
    //int hours   = int(millis / (1000*60*60)) % 24;
    
    std::stringstream ss;
    ss  << setfill('0') << setw(2) << minutes << ":"    << setfill('0') << setw(2) << seconds << ":"    << setfill('0') << setw(precision) << floor((float)ms / 1000.0 * pow(10, precision));
    
	return ss.str();
}

}}
