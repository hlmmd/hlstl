#include "src/hlstl_algobase.h"
#include "src/hlstl_uninitialized.h"
#include "src/hlstl_vector.h"
#include <atomic>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <gtest/gtest.h>
#include <string>
#include <vector>
using namespace hl;
//using namespace std;

std::atomic<int> construct_count;
std::atomic<int> construct_copy_count;
std::atomic<int> construct_movecopy_count;
std::atomic<int> construct_assign_count;
std::atomic<int> construct_moveassign_count;
std::atomic<int> destruct_count;

template <typename T>
struct has_to_string
{
    template <typename C>
    static char test(decltype(&C::to_string));
    template <typename C>
    static int32_t test(...);
    const static bool value = sizeof(test<T>(0)) == sizeof(char);
};

class TestType
{
public:
    TestType() : p_(nullptr), size_(0)
    {
        construct_count++;
    }

    TestType(const char* str) : p_(nullptr), size_(0)
    {
        construct_count++;
        size_ = strlen(str);
        p_ = new char[size_];
        for (int i = 0; i < size_; i++)
            p_[i] = str[i];
    }
    ~TestType()
    {
        destruct_count++;
        delete p_;
        p_ = nullptr;
    }

    TestType(const TestType& another)
    {
        construct_copy_count++;
        p_ = new char[another.size_];
        size_ = another.size_;
        for (int i = 0; i < size_; i++)
            p_[i] = another.p_[i];
    }

    TestType(TestType&& another)
    {
        construct_movecopy_count++;
        if (this == &another)
            return;
        p_ = another.p_;
        size_ = another.size_;
        another.p_ = nullptr;
        another.size_ = 0;
    }

    const TestType& operator=(const TestType& another)
    {
        construct_assign_count++;
        if (this == &another)
            return *this;
        if (p_)
            delete p_;
        p_ = new char[another.size_];
        size_ = another.size_;
        for (int i = 0; i < size_; i++)
            p_[i] = another.p_[i];
        return *this;
    }

    const TestType& operator=(TestType&& another)
    {
        construct_moveassign_count++;
        if (this == &another)
            return *this;
        p_ = another.p_;
        size_ = another.size_;
        another.p_ = nullptr;
        another.size_ = 0;
        return *this;
    }
    std::string to_string() const { return std::string(p_, size_); }

private:
    char* p_;
    int size_;
};

template <typename T, typename Container = hl::vector<T>>
typename std::enable_if<has_to_string<T>::value, std::string>::type
Debug(const Container& container)
{
    std::string logstr = "";
    for (auto& value : container)
        logstr += value.to_string() + " ";
    return logstr;
}

template <typename T, typename Container = hl::vector<T>>
typename std::enable_if<!has_to_string<T>::value, std::string>::type
Debug(const Container& container)
{
    std::string logstr = "";
    for (auto& value : container)
        logstr += std::to_string(value) + " ";
    return logstr;
}

TEST(HLSTL, CONSTRUCT)
{
    vector<int> v1;
    vector<int> v2(1);
    vector<int> v3(2, 2);
    vector<int> v4(v3.begin(), v3.end());
    vector<int> v5(v4);
    vector<int> v6;
    v6 = v5;
    vector<int> v7 = {1, 2, 3};
    LOG(INFO) << Debug<int>(v1);
    LOG(INFO) << Debug<int>(v2);
    LOG(INFO) << Debug<int>(v3);
    LOG(INFO) << Debug<int>(v4);
    LOG(INFO) << Debug<int>(v5);
    LOG(INFO) << Debug<int>(v6);
    LOG(INFO) << Debug<int>(v7);
    vector<int> v8 = std::move(v7);
    LOG(INFO) << Debug<int>(v7);
    LOG(INFO) << Debug<int>(v8);
    vector<int> v9;
    v9 = std::move(v8);
    LOG(INFO) << Debug<int>(v8);
    LOG(INFO) << Debug<int>(v9);
    TestType tt, empty;
    TestType tt2("hello");
    TestType tt3("world");
    TestType tt4(empty);
    tt = tt2;
    tt = tt3;
    tt = empty;
    vector<TestType> vv(2, tt);
    vector<TestType> vv2(2, "aabb");
    vector<TestType> vv3(vv2.begin(), vv2.end());
    vector<TestType> vv4(vv3);
    vector<TestType> vv5;
    vv5 = vv4;
    LOG(INFO) << Debug<TestType>(vv);
    LOG(INFO) << Debug<TestType>(vv2);
    LOG(INFO) << Debug<TestType>(vv3);
    LOG(INFO) << Debug<TestType>(vv4);
    LOG(INFO) << Debug<TestType>(vv5);
    vector<TestType> vv6 = {"aaa", "bbb", "ccc"};
    LOG(INFO) << Debug<TestType>(vv6);
    vector<TestType> vv7(std::move(vv6));
    LOG(INFO) << Debug<TestType>(vv6);
    LOG(INFO) << Debug<TestType>(vv7);
    vector<TestType> vv8;
    vv8 = std::move(vv7);
    LOG(INFO) << Debug<TestType>(vv7);
    LOG(INFO) << Debug<TestType>(vv8);
}

#define DebugHelper(v) \
    LOG(INFO) << "size: " << v.size() << " capacity: " << v.capacity() << " " << Debug<TestType>(v);

TEST(HLSTL, INSERT)
{
    vector<TestType> v;
    for (int i = 0; i < 3; i++)
    {
        v.push_back("x");
        DebugHelper(v);
    }
    v.reserve(10);
    DebugHelper(v);
    v.push_back("x");
    v.push_back("x");
    DebugHelper(v);
    v.resize(11);
    DebugHelper(v);
    v.resize(14, "xx");
    DebugHelper(v);
    v.resize(30);
    DebugHelper(v);
    v.insert(v.begin(), v.begin(), v.end());
    DebugHelper(v);
    v.insert(v.end(), v.begin() + 10, v.end());
    DebugHelper(v);
    v.resize(10);
    DebugHelper(v);
    v.shrink_to_fit();
    DebugHelper(v);
    v.erase(v.begin());
    DebugHelper(v);
    v.erase(v.begin() + 2, v.begin() + 7);
    DebugHelper(v);
    v.resize(2);
    DebugHelper(v);
}
void my_exit(void)
{
    LOG(INFO) << "construct_count " << construct_count;
    LOG(INFO) << "construct_copy_count " << construct_copy_count;
    LOG(INFO) << "construct_movecopy_count " << construct_movecopy_count;
    LOG(INFO) << "construct_assign_count " << construct_assign_count;
    LOG(INFO) << "construct_moveassign_count " << construct_moveassign_count;
    LOG(INFO) << "destruct_count " << destruct_count;
}
int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    google::ParseCommandLineFlags(&argc, &argv, true);
    google::ShutDownCommandLineFlags();
    int rtn = RUN_ALL_TESTS();
    ::atexit(my_exit);
    return rtn;
}
