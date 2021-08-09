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
    hl::uninitialized_fill<int*, int>(xxx, xxx + 20, 10);
    char ccc[10];
    hl::uninitialized_fill_n<char*, int, char>(ccc, 10, 'a');
}

TEST(HLSTL, VECTOR_INT)
{
    {
        hl::vector<int> nums;
    }
    {
        hl::vector<int> nums(2);
    }
    {
        hl::vector<int> nums(2, 2);
        LOG(INFO) << nums.size();
        for (int i = 0; i < nums.size(); i++)
            LOG(INFO) << nums[i];
        for (auto it = nums.begin(); it != nums.end(); ++it)
            LOG(INFO) << *it;
    }
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    google::ParseCommandLineFlags(&argc, &argv, true);
    google::ShutDownCommandLineFlags();
    return RUN_ALL_TESTS();
}