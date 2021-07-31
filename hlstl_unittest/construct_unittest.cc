#include "src/hlstl_allocator.h"
#include "src/hlstl_construct.h"
#include <iostream>

using namespace std;

int main()
{
    hl::Allocator<int> alloc;
    int* p = static_cast<int*>(alloc.Allocate(1000));
    hl::Construct(p, 1);

    cout << *p << endl;

    hl::Destroy(p);
    alloc.DeAllocate(p);
    return 0;
}