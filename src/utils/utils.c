#include "utils.h"
#include "../memory/memory.h"

int Utils__compare_segment(void *x, void *y) {
    Segment *seg1 = (Segment *) x;
    Segment *seg2 = (Segment *) y;

    if (seg1->seg_id == seg2->seg_id)
        return 0;
    else
        return 1;
}

int Utils__compare_process(void *x, void *y) {
    Process *proc1 = (Process *) x;
    Process *proc2 = (Process *) y;

    if (proc1->pid == proc2->pid)
        return 0;
    else
        return 1;
}

int Utils__compare_page(void *x, void *y) {
    Page *page1 = (Page *) x;
    Page *page2 = (Page *) y;

    if (page1->page_id == page2->page_id)
        return 0;
    else
        return 1;
}
