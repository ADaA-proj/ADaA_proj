#include <iostream>

int main()
{
    long long *a = new long long;
    *a = ~(1ll << 63);
    std::cout << *((double *)a);
}