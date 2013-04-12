#ifndef COMMON_H
#define COMMON_H

#define NUM_KEYWORDS 3
#define KEYWORDS {"Xrange", "Yrange", "Initial"}

enum cell {DEAD, ALIVE};

struct range_t
{
    int low;
    int high;
};
#endif
