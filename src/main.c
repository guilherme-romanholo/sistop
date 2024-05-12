#include "kernel/kernel.h"
#include "terminal/terminal.h"

int main(int argc, char *argv[])
{
    Kernel__create();
    Terminal__init();
    return 0;
}
