#include "kernel/kernel.h"

int main(int argc, char *argv[])
{
    Kernel__create();
    Kernel__syscall(CREATE_PROCESS, "synt");
}
