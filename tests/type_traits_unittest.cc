#include "src/hlstl_helper.h"
#include "src/hlstl_type_traits.h"
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <gtest/gtest.h>

template <typename T>
struct simpile_type_test
{
    using has_trivial_default_constructor = typename hl::__type_traits<T>::has_trivial_default_constructor;
    using has_trivial_copy_constructor = typename hl::__type_traits<T>::has_trivial_copy_constructor;
    using has_trivial_assignment_operator = typename hl::__type_traits<T>::has_trivial_assignment_operator;
    using has_trivial_destructor = typename hl::__type_traits<T>::has_trivial_destructor;
    using is_POD_type = typename hl::__type_traits<T>::is_POD_type;
    operator bool()
    {
        if (hl::is_same_type<has_trivial_default_constructor, hl::__true_type>() &&
            hl::is_same_type<has_trivial_copy_constructor, hl::__true_type>() &&
            hl::is_same_type<has_trivial_assignment_operator, hl::__true_type>() &&
            hl::is_same_type<has_trivial_destructor, hl::__true_type>() &&
            hl::is_same_type<is_POD_type, hl::__true_type>())
            return true;
        else
            return false;
    }
};

struct Foo
{
};

TEST(HLSTL, TYPE_TRAITS)
{
    ASSERT_EQ(bool(simpile_type_test<bool>()), true);
    ASSERT_EQ(bool(simpile_type_test<char>()), true);
    ASSERT_EQ(bool(simpile_type_test<unsigned char>()), true);
    ASSERT_EQ(bool(simpile_type_test<short>()), true);
    ASSERT_EQ(bool(simpile_type_test<unsigned short>()), true);
    ASSERT_EQ(bool(simpile_type_test<int>()), true);
    ASSERT_EQ(bool(simpile_type_test<unsigned int>()), true);
    ASSERT_EQ(bool(simpile_type_test<long>()), true);
    ASSERT_EQ(bool(simpile_type_test<unsigned long>()), true);
    ASSERT_EQ(bool(simpile_type_test<long long>()), true);
    ASSERT_EQ(bool(simpile_type_test<unsigned long long>()), true);
    ASSERT_EQ(bool(simpile_type_test<float>()), true);
    ASSERT_EQ(bool(simpile_type_test<double>()), true);
    ASSERT_EQ(bool(simpile_type_test<long double>()), true);
    ASSERT_EQ(bool(simpile_type_test<Foo*>()), true);
    ASSERT_EQ(bool(simpile_type_test<Foo>()), false);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    google::ParseCommandLineFlags(&argc, &argv, true);
    return RUN_ALL_TESTS();
}
