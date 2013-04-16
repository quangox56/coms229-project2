#ifndef COMMON_H
#define COMMON_H

#define NUM_KEYWORDS 6
#define REQ_KEYWORDS 3 //Only Xrange, Yrange, and Initial are required in an aut
#define KEYWORDS {"Xrange", "Yrange", "Initial", "Name", "Chars", "Colors"}

enum cell {DEAD, ALIVE};

struct range_t
{
    int low;
    int high;
};
#endif
