#include<iostream>
#include<sstream>
#include<vector>
#include<cstdlib>
#include<string>

enum cell {DEAD, ALIVE};
using namespace std;

int main()
{
    char leftBracket;
    char Y;
    char equals;
    char comma;
    char colon;
    char rightBracket;
    char semicolon;
    int y;
    int x;
    vector< vector<cell> > cells;
    string stringValues = "{\nY = 2 :1;\nY=1:2, 10;Y=10:2,3,4;\n};";
    istringstream iss(stringValues);

    //Parse the begining of the compound statement
    if((!(iss >> leftBracket)))
    {
        cerr << "There was an error in the Initial statement." << endl;
        cerr << "Program is now exiting with error." << endl;
        exit(1);
    }
    else
    {
        if(leftBracket != '{')
        {
            cerr << "There was an error in the Initial statement." << endl;
            cerr << "Program is now exiting with error." << endl;
            exit(1);
        }
    }

    while(1)
    {
        //parse the Y value
        if((!(iss>>Y) || !(iss>>equals) || !(iss>>y) || !(iss>>colon)))
        {
            cerr << "There was an error in the Initial statement." << endl;
            cerr << "Program is now exiting with error." << endl;
            exit(1);
        }
        else
        {
            if(Y != 'Y' || equals != '=' || colon != ':')
            {
                cerr << "There was an error in the Initial statement." << endl;
                cerr << "Program is now exiting with error." << endl;
                exit(1);
            }
        }

        //Begin parsing corresponding x-list
        do
        {
            if(!(iss>>x) || !(iss>>comma))
            {
                cerr << "There was an error in the Initial statement." << endl;
                cerr << "Program is now exiting with error." << endl;
                exit(1);
            }
            else
            {
                if(comma != ',' && comma != ';')
                {
                    cerr << "There was an error in the Initial statement." << endl;
                    cerr << "Program is now exiting with error." << endl;
                    exit(1);
                }
                cells[y][x] = ALIVE;
            }
        }while(comma != ';');

        if(!(iss>>rightBracket))
        {
            cerr << "There was an error in the Initial statement." << endl;
            cerr << "Program is now exiting with error." << endl;
            exit(1);
        }
        else
        {
            if(rightBracket == '}')
            {//if we have reached the right bracket then exit the loop
                break;
            }
            else
            {//Else put whatever we popped back on for the next loop
                iss.unget();
            }

        }
    }
    if(!(iss>>semicolon))
    {
        cerr << "There was an error in the Initial statement." << endl;
        cerr << "Program is now exiting with error." << endl;
        exit(1);
    }
    else
    {
        if(semicolon != ';')
        {
            cerr << "There was an error in the Initial statement." << endl;
            cerr << "Program is now exiting with error." << endl;
            exit(1);
        }
    }

    cout << cells[1][2] << endl;
    cout << cells[1][10] << endl;
    cout << cells[2][1] << endl;
    cout << cells[10][2] << endl;
    cout << cells[10][3] << endl;
    cout << cells[10][4] << endl;
    cout << cells[5][5] << endl;

    return 0;
}
