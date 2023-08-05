#ifndef UTILS3D_H
#define UTILS3D_H

namespace Utils3D
{
    template <typename T>  T lerp(const T& var1, const T& var2, double percent)
    {
        T res = var1 + (var2 - var1) * percent;
        return res;
    }
}

#endif // UTILS3D_H
