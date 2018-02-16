#pragma once

namespace sb { namespace utils {  

using namespace std;
    
inline std::string millisToTimestamp( int millis, int precision = 3 ){
    int ms = millis % 1000;
    int seconds = int(millis / 1000) % 60 ;
    int minutes = int(millis / (1000*60));
    //int hours   = int(millis / (1000*60*60)) % 24;
    
    stringstream ss;
    ss  << setfill('0') << setw(2) << minutes << ":"
    << setfill('0') << setw(2) << seconds << ":"
    << setfill('0') << setw(precision) << ms;
    
    return ss.str();
}
//
//std::string toTimestamp( int millis ){
//    //    millis = 755020;
//    // millis = 12048;
//
//    int ms = millis % 1000;
//    int seconds = int(millis / 1000) % 60 ;
//    int minutes = int(millis / (1000*60));
//    //int hours   = int(millis / (1000*60*60)) % 24;
//
//    stringstream ss;
//    ss  << setfill('0') << setw(2) << minutes << ":"
//    << setfill('0') << setw(2) << seconds << ":"
//    << setfill('0') << setw(3) << ms;
//
//    return ss.str();
//}



}}
