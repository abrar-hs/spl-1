#ifndef M3_RESULT_H
#define M3_RESULT_H

#include <string>

namespace m3 {
    struct Status {
        bool ok = true;
        std::string error;
    };
}

#endif