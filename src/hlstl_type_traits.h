#ifndef HLSTL_TYPE_TRAITS_H
#define HLSTL_TYPE_TRAITS_H

namespace hl
{

struct __true_type
{
};

struct __false_type
{
};

template <typename T>
struct __type_traits
{
    using this_dummy_member_must_be_first = __true_type;
    /* Do not remove this member. It informs a compiler which
       automatically specializes __type_traits that this
       __type_traits template is special. It just makes sure that
       things work if an implementation is using a template
       called __type_traits for something unrelated. */

    /* The following restrictions should be observed for the sake of
      compilers which automatically produce type specific specializations
      of this class:
          - You may reorder the members below if you wish
          - You may remove any of the members below if you wish
          - You must not rename members without making the corresponding
            name change in the compiler
          - Members you add will be treated like regular members unless
            you add the appropriate support in the compiler. */

    using has_trivial_default_constructor = __false_type;
    using has_trivial_copy_constructor = __false_type;
    using has_trivial_assignment_operator = __false_type;
    using has_trivial_destructor = __false_type;
    using is_POD_type = __false_type;
};

template <>
struct __type_traits<bool>
{
    using has_trivial_default_constructor = __true_type;
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};

template <>
struct __type_traits<char>
{
    using has_trivial_default_constructor = __true_type;
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};

template <>
struct __type_traits<unsigned char>
{
    using has_trivial_default_constructor = __true_type;
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};

template <>
struct __type_traits<short>
{
    using has_trivial_default_constructor = __true_type;
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};

template <>
struct __type_traits<unsigned short>
{
    using has_trivial_default_constructor = __true_type;
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};

template <>
struct __type_traits<int>
{
    using has_trivial_default_constructor = __true_type;
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};

template <>
struct __type_traits<unsigned int>
{
    using has_trivial_default_constructor = __true_type;
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};

template <>
struct __type_traits<long>
{
    using has_trivial_default_constructor = __true_type;
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};

template <>
struct __type_traits<unsigned long>
{
    using has_trivial_default_constructor = __true_type;
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};

template <>
struct __type_traits<long long>
{
    using has_trivial_default_constructor = __true_type;
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};

template <>
struct __type_traits<unsigned long long>
{
    using has_trivial_default_constructor = __true_type;
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};

template <>
struct __type_traits<float>
{
    using has_trivial_default_constructor = __true_type;
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};

template <>
struct __type_traits<double>
{
    using has_trivial_default_constructor = __true_type;
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};

template <>
struct __type_traits<long double>
{
    using has_trivial_default_constructor = __true_type;
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};

// 指针类型偏特化
template <typename T>
struct __type_traits<T*>
{
    using has_trivial_default_constructor = __true_type;
    using has_trivial_copy_constructor = __true_type;
    using has_trivial_assignment_operator = __true_type;
    using has_trivial_destructor = __true_type;
    using is_POD_type = __true_type;
};

} // namespace hl
#endif
