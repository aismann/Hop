#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <Collision/collisionMesh.h>

namespace Hop::System::Physics
{
    struct Rectangle;
}

using Hop::System::Physics::Rectangle;

namespace Hop::Maths
{
    template <class T>
    void rotateClockWise(T & x, T & y, T & cosine, T & sine)
    {
        T xt, yt;
        xt = x;
        yt = y;

        x = xt*cosine + yt*sine;
        y = yt*cosine - xt*sine; 
    }

    void rotateClockWise(Rectangle * r, double cosine, double sine);

    void scale(Rectangle * r, double s);

    void translate(Rectangle * r, double x, double y);

}

#endif /* TRANSFORM_H */