#include"common.h"

using namespace std;

void exitWithErr(string err)
{
    cerr << err << endl;
    cerr << "Program is now exiting with error." << endl;
    exit(1);
}
