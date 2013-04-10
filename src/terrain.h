#ifndef TERRAIN_H
#define TERRAIN_H

#include<iostream>
#include<sstream>
#include<cstdlib>
#include<vector>
#include<string>

#define NUM_KEYWORDS 3
#define KEYWORDS {"Xrange", "Yrange", "Initial"}


enum cell {DEAD, ALIVE};

class terrain
{
    private:
    //private member variables
    vector< vector<cell> > cells; //rows are y, columns are x
    int xRangeLow;
    int xRangeHigh;
    int yRangeLow;
    int yRangeHigh;
    bool isValid;




    //private member functions
    void handleKeyword(istringstream&);

    void resizeCells(size_t newSize);

    public:
    //public member functions
    terrain();
    istream& operator>>(istream& in);

    

}
#endif
