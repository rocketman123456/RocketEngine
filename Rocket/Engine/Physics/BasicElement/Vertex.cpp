#include "Physics/BasicElement/Vertex.h"
#include "Physics/BasicElement/Parameter.h"

namespace Rocket {
    bool Vertex::IsCoincidentWith(const Vertex& v) {
        if(id == v.id)   // Check ID
            return true;
        else if((position - v.position).squaredNorm() < EPS * EPS)  // Check Position
            return true;
        else
            return false;
    }

    bool Vertex::operator==(const Vertex& v) {
        return IsCoincidentWith(v);
    }
}