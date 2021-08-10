#include "src/hlstl_allocator.h"
#include "src/hlstl_construct.h"
#include <iostream>

using namespace std;

int main()
{
    hl::Allocator<int> alloc;
    int* p = static_cast<int*>(alloc.allocate(1000));
    hl::construct(p, 1);

    cout << *p << endl;

    hl::destroy(p);
    alloc.deallocate(p);
    return 0;
}
