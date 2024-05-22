#include "kernel/kernel.h"
#include "terminal/terminal.h"
#include "interface/interface.h"

int main(int argc, char *argv[])
{
    Kernel__create();
    // Terminal__init();
    Interface__create();
    return 0;
}
