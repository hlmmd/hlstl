#include "src/hlstl_allocator.h"
#include <iostream>

using namespace std;

int main()
{
    hl::Allocator<int> alloc;
    int* p = static_cast<int*>(alloc.Allocate(1000));
    alloc.DeAllocate(p);
    return 0;
}