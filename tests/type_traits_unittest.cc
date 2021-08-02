#include "src/hlstl_type_traits.h"
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <gtest/gtest.h>

using namespace std;
TEST(HLSTL, TYPE_TRAITS)
{
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    google::ParseCommandLineFlags(&argc, &argv, true);
    return RUN_ALL_TESTS();
}
