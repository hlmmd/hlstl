#include "src/hlstl_allocator.h"
#include <iostream>

using namespace std;

int main()
{
    hl::Allocator<int> alloc;
    int* p = static_cast<int*>(alloc.allocate(1000));
    alloc.deallocate(p);
    return 0;
}