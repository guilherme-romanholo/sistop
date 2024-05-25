#include "kernel/kernel.h"
#include "interface/interface.h"

int main(int argc, char *argv[])
{
    Kernel__create();
    Interface__init();
    return 0;
}
