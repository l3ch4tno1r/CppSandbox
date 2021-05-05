#include <iostream>
#include <assert.h>

class A
{
public:
    ~A()
    {
        x = 10;
    }

    union
    {
        int x;
        char c[4];
    };
};

int test()
{
    A a;
    a.c[0] = 'a';
    a.c[1] = 'b';
    a.c[2] = 'c';
    a.c[3] = 'd';
    return a.x;
}

int main(int argc, char* argv[])
{
    int x = test();
    assert(x == 5);

    std::cin.get();

    return 0;
}