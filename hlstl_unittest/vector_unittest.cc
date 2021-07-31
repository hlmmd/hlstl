#include "src/hlstl_vector.h"
#include <iostream>

using namespace std;

int main()
{
    hl::vector<int> nums(10);
	for(auto it = nums.begin(); it!=nums.end(); it++)
	{
		std::cout << "xxx" << *it << std::endl;
	}

    return 0;
}