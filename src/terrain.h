#define NUM_KEYWORDS 3
#define KEYWORDS {"Xrange", "Yrange", "Initial"}

enum cell {DEAD, ALIVE};

class terrain
{
    private:
    cell cells[][]; //rows are y, columns are x
    int xRangeLow;
    int xRangeHigh;
    int yRangeLow;
    int yRangeHigh;

    void parseAut();





    void handleKeyword(istringstream&);

    public:
    istream& operator>>(istream& in, terrain &cTerrain);

    

}
