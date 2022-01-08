#include "Math/CommonMath.h"

namespace Rocket {
    // From Quake 3 Source Code
    float CarmSqrt(float x) {
        union{
            int intPart;
            float floatPart;
        } convertor;
        union{
            int intPart;
            float floatPart;
        } convertor2;
        convertor.floatPart = x;
        convertor2.floatPart = x;
        convertor.intPart = 0x1FBCF800 + (convertor.intPart >> 1);
        convertor2.intPart = 0x5f3759df - (convertor2.intPart >> 1);
        return 0.5f*(convertor.floatPart + (x * convertor2.floatPart));
    }
}