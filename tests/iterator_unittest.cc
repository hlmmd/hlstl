#include "src/hlstl_helper.h"
#include "src/hlstl_iterator_base.h"
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <gtest/gtest.h>

template <typename IteratorTag, typename T>
struct IteratorTagTester
{
    operator bool()
    {
        hl::iterator<IteratorTag, T> iter;
        return hl::is_same_type<IteratorTag, decltype(hl::iterator_category(iter))>();
    }
};

TEST(HLSTL, ITERATOR_TAG)
{
    bool isSame = IteratorTagTester<hl::input_iterator_tag, int>();
    ASSERT_EQ(isSame, true);
    isSame = IteratorTagTester<hl::output_iterator_tag, int>();
    ASSERT_EQ(isSame, true);
    isSame = IteratorTagTester<hl::forward_iterator_tag, int>();
    ASSERT_EQ(isSame, true);
    isSame = IteratorTagTester<hl::bidirectional_iterator_tag, int>();
    ASSERT_EQ(isSame, true);
    isSame = IteratorTagTester<hl::random_access_iterator_tag, int>();
    ASSERT_EQ(isSame, true);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    google::ParseCommandLineFlags(&argc, &argv, true);
    return RUN_ALL_TESTS();
}
