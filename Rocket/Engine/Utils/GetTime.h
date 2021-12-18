#pragma once

#include <string>
#include <time.h>

namespace Rocket {
    static std::string CurrentDate(){ 
        time_t now = time(NULL); 
        struct tm tstruct; 
        char buf[40]; 
        tstruct = *localtime(&now); 
        //format: day DD-MM-YYYY 
        strftime(buf, sizeof(buf), "%A %d/%m/%Y", &tstruct); 
        return buf; 
    }

    static std::string CurrentTime(){ 
        time_t now = time(NULL); 
        struct tm tstruct; 
        char buf[40]; 
        tstruct = *localtime(&now); 
        //format: HH:mm:ss 
        strftime(buf, sizeof(buf), "%X", &tstruct); 
        return buf; 
    }
}