#ifndef HLSTL_HELPER_H
#define HLSTL_HELPER_H

namespace hl
{

template <typename T1, typename T2>
struct is_same_type
{
    operator bool() const
    {
        return false;
    }
};

template <typename T1>
struct is_same_type<T1, T1>
{
    operator bool() const
    {
        return true;
    }
};

} // namespace hl

#endif