#include "utils.h"
#include "../disk/disk.h"
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

int Utils__compare_tracks_up(void *x, void *y) {
    DiskRequest *d1 = (DiskRequest *) x;
    DiskRequest *d2 = (DiskRequest *) y;

    if (d1->track > d2->track)
        return 1;
    else
        return 0;
}

int Utils__compare_tracks_down(void *x, void *y) {
    DiskRequest *d1 = (DiskRequest *) x;
    DiskRequest *d2 = (DiskRequest *) y;

    if (d1->track < d2->track)
        return 1;
    else
        return 0;
}
