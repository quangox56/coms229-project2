#include"terrain.h"

using namespace std;

istream& operator>>(istream& in, terrain& cTerrain)
{
    string autFile;

    string tmp;
    while(in >> tmp)//Put the entire aut file into a string
    {
        autFile += tmp;
    }

    int lineCount = 1;//The file obviously starts with one line
    size_t found = 0;
    //count the number of lines
    while(found = autFile.find("\n", found) != string::npos)
    {
        lineCount++;
    }

    //This string will hold each line. This is necessary to simplify comments.
    string* autFileLines = (string*)malloc(sizeof(string)*lineCount);

    int index = 0;
    unsigned lastFound = 0;
    found = 0;
    //Split the file into lines.
    while(found = autFile.find("\n", found) != string::npos)
    {
        autFileLines[index] = autFile.substr(lastFound, found-lastFound);
        lastFound = found;
        index++;
    }
    autFileLines[index] = autFile.substr(lastFound);

    int semicolonCount = 0;
    found = 0;
    //Find comment symbol # and remove all comments.
    //Also count all semicolons while looping through all the lines.
    for(int i = 0; i < lineCount; i++)
    {
        found = autFileLines[i].find("#");
        if(found != string::npos)
        {
            autFileLines[i] = autFileLines[i].substr(0, found);
            autFileLines[i] += " ";//This is necessary because a comment
                                   //can count as whitespace because of the
                                   //newline.
        }
        found = 0;
        while(found = autFileLines[i].find(";", found) != string::npos)
        {
            semicolonCount++;
        }
    }

    //This variable will be an array of all the statements in the file.
    //Statements are delimited by semicolons. All Comments have been removed
    //by this point.
    string* autFileStatements = (string*)malloc(sizeof(string)*semicolonCount);
    index = 0;
    //Divide the lines into statements delimited by semicolons
    for(int i = 0; i < lineCount; i++)
    {
        lastFound = 0;
        found = 0;
        found = autFileLines[i].find(";");
        if(found != string::npos)
        {
            autFileStatements[index] = autFileLines[i].substr(lastFound, found);
            lastFound = found;
            index++;
            while(found = autFileLines[i].find(";", found) != string::npos)
            {
                autFileStatements[index] += autFileLines[i].substr(lastFound, found);
                index++;
                lastFound = found;
            }
            autFileStatements[index] += autFileLines[i].substr(lastFound);
        } 
        else//there was no semicolon, add the entire line to the statement.
        {
            autFileStatements[index] += autFileLines[i];
        }
    }
    free(autFileLines);//No longer need the lines. They have served their
    //purpose of removing all comments.

    //use an istringstream to parse the statements with whitespace delimiting
    istringstream* iss = new istringstream(autFileStatements[0]);
    string keywords[NUM_KEYWORDS] = KEYWORDS;
    bool keywordsFound[NUM_KEYWORDS];
    for(int i = 0; i < semicolonCount; i++)
    {
        string tmp;
        bool keywordFound = false;
        (*iss) >> tmp;
        for(int j = 0; j < NUM_KEYWORDS; i++)
        {
            if(tmp == keywords[j])
            {
                keywordFound = true;
                keywordsFound[j] = true;
            }
        }
        if(keywordFound)
        {
            //If a keyword was found then call the helper function
            cTerrain.handleKeyword(*iss, tmp);
        }
        delete iss;
        iss = new istringstream(autFileStatements[i+1]);
    }
    delete iss;

    //Check that all required keywords were found.
    for(int i = 0; i < NUM_KEYWORDS; i++)
    {
        if(!keywordsFound[i])
        {
            cerr << "Error: Not all required keywords found in .aut file." << endl;
            cerr << "Program exiting in error." << endl;
            exit(1);
        }
    }

    cTerrain.isValid = true;
    return in;
}

ostream& operator<<(ostream& out, terrain& cTerrain)
{
    if(cTerrain.printAut)
    {
        out << "Xrange " << cTerrain.xRangeLow << " " << cTerrain.xRangeHigh;
        out << "Yrange " << cTerrain.yRangeLow << " " << cTerrain.yRangeHigh;
        out << "Initial {" << endl;
        
        //TODO: finish this
            

    }
    else
    {
        for(int y = cTerrain.yRangeLow; y < cTerrain.yRangeHigh; y++)
        {
            for(int x = cTerrain.xRangeLow; x < cTerrain.xRangeLow; x++)
            {
                out << cTerrain.cells[y][x] ? "1":"~";
            }
            out << endl;
        }
    }

}

void terrain::resizeCells(size_t newSize)
{
    if(newSize > cells.capacity())
    {
        cells.resize(newSize);
        for(int i = 0; i < cells.size(); i++)
        {
            cells[i].resize(newSize);
        }
    }
}

void terrain::handleKeyword(istringstream& iss, string keyword)
{
    if(keyword == "Xrange")
    {
        if(!xRangeSet)//If the xRange has already been set, overrule the .aut
        {
            char semicolon;
            if((!(iss >> xRangeLow) || !(iss >> xRangeHigh) || !(iss >> semicolon)))
            {
                cerr << "There was an error in the xRange statement." << endl;
                cerr << "Program is now exiting with error." << endl;
                exit(1);
            }
            if(semicolon != ';')
            {
                cerr << "There was an error in the xRange statement." << endl;
                cerr << "Program is now exiting with error." << endl;
                exit(1);
            }
        }
    }
    else if(keyword == "Yrange")
    {
        if(!yRangeSet)//If the yRange has already been set, overrule the .aut
        {
            char semicolon;
            if((!(iss >> yRangeLow) || !(iss >> yRangeHigh) || !(iss >> semicolon)))
            {
                cerr << "There was an error in the yRange statement." << endl;
                cerr << "Program is now exiting with error." << endl;
                exit(1);
            }
            if(semicolon != ';')
            {
                cerr << "There was an error in the yRange statement." << endl;
                cerr << "Program is now exiting with error." << endl;
                exit(1);
            }
        }
    }
    else if(keyword == "Initial")
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

        int xSize = xRangeHigh-xRangeLow;
        int ySize = yRangeHigh-yRangeLow;
        resizeCells(xSize > ySize ? xSize+2:ySize+2);//Size the vectors
        //to the largest of the two.

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

        if(!(iss >> rightBracket))
        {
            cerr << "There was an error in the Initial statement." << endl;
            cerr << "Program is now exiting with error." << endl;
            exit(1);
        }
        else
        {
            if(rightBracket != '}')
            {//if we pulled a right bracket off, it's an empty initial statement.
                //Else put whatever we took out of the stream back.
                //and continue processing.
                iss.unget();
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

                            if(y >=yRangeLow && y <= yRangeHigh &&
                                    x >=xRangeLow && x <= xRangeHigh)
                            {
                                cells[y-yRangeLow][x-xRangeLow] = ALIVE;
                            }
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
            }

        }
    }
    else
    {
        cerr << "Shouldnt reach here, invalid keyword passed.\n";
    }
}

terrain::terrain()
{
    isValid = false;
    printAut = false;
    xRangeSet = false;
    yRangeSet = false;
}
    
void terrain::setYRange(range_t yRange)
{
    yRangeLow = yRange.low;
    yRangeHigh = yRange.high;
    yRangeSet = true;
}

void terrain::setXRange(range_t xRange)
{
    xRangeLow = xRange.low;
    xRangeHigh = xRange.high;
    xRangeSet = true;
}

void terrain::setPrintModeAut(bool _printAut)
{
    printAut = _printAut;
}

void terrain::simulate(int cycles)
{
    while(cycles > 0)
    {
        for(int y = yRangeLow; y <= yRangeHigh; y++)
        {
            for(int x = xRangeLow; x <= xRangeHigh; x++)
            {
                cells[y][x] = getNextState(x,y);
            }
        }
        cycles--;
    }
}

int terrain::numberOfLiveNeighbors(int x, int y)
{
    int liveNeighbors = 0;
    for(int i = y-1; i <= y+1; i++)
    {
        for(int j = x-1; j <= x+1; j++)
        {
            //The cell can't be it's own neighbor
            if(i != y && x != j)
            {
                liveNeighbors += cells[i-yRangeLow][j-xRangeLow];
            }
        }
    }

    return liveNeighbors;
}

cell terrain::getNextState(int x, int y)
{
    int liveNeighbors = numberOfLiveNeighbors(x, y);

    cell returnState;

    if(cells[y-yRangeLow][x-xRangeLow] == ALIVE)
    {
        if(liveNeighbors == 2 || liveNeighbors == 3)
        {
            returnState = ALIVE;
        }
        else
        {
            returnState = DEAD;
        }
    }
    else//Cell is dead
    {
        if(liveNeighbors == 3)
        {
            returnState = ALIVE;
        }
        else
        {
            returnState = DEAD;
        }
    }
}
