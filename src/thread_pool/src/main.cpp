
#include <XThreadPool.h>

#include <iostream>


int main(int argc, char *argv[])
{
    XThreadPool pool;
    pool.init(16);
    pool.start();

    getchar();

    return 0;
}
