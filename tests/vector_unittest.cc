#include "src/hlstl_uninitialized.h"
#include "src/hlstl_vector.h"
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <gtest/gtest.h>

TEST(HLSTL, UNINITIALIZED)
{
    int xxx[20];
    for (int i = 0; i < 10; i++)
        xxx[i] = i;

    hl::uninitialized_copy<int*, int*>(xxx, xxx + 10, xxx + 10);
    for (int i = 0; i < 20; i++)
        LOG(INFO) << xxx[i];
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    google::ParseCommandLineFlags(&argc, &argv, true);
    return RUN_ALL_TESTS();
}