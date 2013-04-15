#include"terrain.h"

using namespace std;

istream& operator>>(istream& in, terrain& cT)
{
    string autFile;

    string tmp;
    while(getline(in, tmp))//Put the entire aut file into a string
    {
        autFile += tmp + "\n";
    }

    int lineCount = 0;
    size_t found = 0;
    //count the number of lines
    while((found = autFile.find("\n", found)) != string::npos)
    {
        lineCount++;
        found++;
    }


    //This string will hold each line. This is necessary to simplify comments.
    string* autFileLines = new string[lineCount];

    int index = 0;
    size_t lastFound = 0;
    found = 0;
    //Split the file into lines.
    while((found = autFile.find("\n", found)) != string::npos)
    {
        autFileLines[index] = autFile.substr(lastFound, found-lastFound+1);
        found++;
        lastFound = found;
        index++;
    }
    //autFileLines[index] = autFile.substr(lastFound);

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
        while((found = autFileLines[i].find(";", found)) != string::npos)
        {
            semicolonCount++;
            found++;
        }
    }

    //This variable will be an array of all the statements in the file.
    //Statements are delimited by semicolons. All Comments have been removed
    //by this point.
    string* autFileStatements = new string[semicolonCount];
    index = 0;
    //Divide the lines into statements delimited by semicolons
    for(int i = 0; i < lineCount; i++)
    {
        lastFound = 0;
        found = 0;
        found = autFileLines[i].find(";");
        if(found != string::npos)
        {
            autFileStatements[index] += autFileLines[i].substr(lastFound, found-lastFound+1);
            found++;
            lastFound = found;
            index++;
            while((found = autFileLines[i].find(";", found)) != string::npos)
            {
                autFileStatements[index] += autFileLines[i].substr(lastFound, found-lastFound+1);
                index++;
                found++;
                lastFound = found;
            }
            //autFileStatements[index] += autFileLines[i].substr(lastFound);
        } 
        else//there was no semicolon, add the entire line to the statement.
        {
            autFileStatements[index] += autFileLines[i];
        }
    }
    delete[] autFileLines;//No longer need the lines. They have served their
    //purpose of removing all comments.

    //Trim any beginning whitespace, this can screw up later parsing.
    for(int i = 0; i < semicolonCount; i++)
    {
        int j = 0;
        while(autFileStatements[i][j] == ' ' || 
              autFileStatements[i][j] == '\n' || 
              autFileStatements[i][j] == '\t' || 
              autFileStatements[i][j] == '\v' || 
              autFileStatements[i][j] == '\r' || 
              autFileStatements[i][j] == '\f')
        {
            autFileStatements[i] = autFileStatements[i].substr(1);
        }
    }

    
    //use an istringstream to parse the statements with whitespace delimiting
    istringstream* iss = new istringstream(autFileStatements[0]);
    string keywords[NUM_KEYWORDS] = KEYWORDS;
    bool keywordsFound[NUM_KEYWORDS];
    for(int i = 0; i < semicolonCount; i++)
    {
        string tmp;
        bool keywordFound = false;
        (*iss) >> tmp;
        for(int j = 0; j < NUM_KEYWORDS; j++)
        {
            found = 0;
            if((found = tmp.find(keywords[j])) != string::npos)
            {
                if(found == 0)//Keyword must be at the start of the statement
                {
                    if(keywords[j] == "Initial")
                    {
                        delete iss;
                        i++;
                        string cmpdStatement = autFileStatements[i-1] + autFileStatements[i];
                        while(autFileStatements[i].find("}") == string::npos)
                        {
                            if(i < semicolonCount)
                            {
                                cmpdStatement += autFileStatements[i];
                                i++;
                            }
                            else
                            {
                                break;
                            }
                        }
                        if(i < semicolonCount)
                        {
                            cmpdStatement += autFileStatements[i];
                        }

                        //Remove the keyword from the string.
                        cmpdStatement = cmpdStatement.substr(keywords[j].length());
                        tmp = "Initial";
                        iss = new istringstream(cmpdStatement);
                    }
                    keywordFound = true;
                    keywordsFound[j] = true;
                }
            }
        }
        if(keywordFound)
        {
            //If a keyword was found then call the helper function
            cT.handleKeyword(*iss, tmp);
        }
        delete iss;
        if((i+1) < semicolonCount)
        {
            iss = new istringstream(autFileStatements[i+1]);
        }
    }
    delete[] autFileStatements;

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

    cT.isValid = true;
    return in;
}

ostream& operator<<(ostream& out, terrain& cT)
{
    if(!cT.wxRangeSet)
    {
        cT.wxRangeLow = cT.xRangeLow;
        cT.wxRangeHigh = cT.xRangeHigh;
    }
    if(!cT.wyRangeSet)
    {
        cT.wyRangeLow = cT.yRangeLow;
        cT.wyRangeHigh = cT.yRangeHigh;
    }

    if(cT.printAut)
    {
        out << "Xrange " << cT.wxRangeLow << " " << cT.wxRangeHigh << ";" << endl;
        out << "Yrange " << cT.wyRangeLow << " " << cT.wyRangeHigh << ";" << endl;
        out << "Initial {" << endl;

        for(int y = (cT.wyRangeLow - cT.yRangeLow) + (cT.wyRangeHigh - cT.wyRangeLow); 
            y >= cT.wyRangeLow - cT.yRangeLow;
            y--)
        {
            out << "Y = " << y << " : ";
            bool firstX = true;
            for(int x = cT.wxRangeLow - cT.xRangeLow; 
                    x <= (cT.wxRangeLow - cT.xRangeLow) + (cT.wxRangeHigh - cT.wxRangeLow);
                    x++)
            {
                if(0 <= y && y <= cT.yRangeHigh-cT.yRangeLow &&
                   0 <= x && x <= cT.xRangeHigh-cT.xRangeLow)
                {
                    if(cT.cells[y][x] == ALIVE)
                    {
                        if(firstX)
                        {
                            out << x;
                            firstX = false;
                        }
                        else
                        {
                            out << "," << x;
                        }
                    }
                }
            }
            out << ";" << endl;
        }
        out << "};" << endl;

    }
    else
    {
        for(int y = (cT.wyRangeLow - cT.yRangeLow) + (cT.wyRangeHigh - cT.wyRangeLow); 
            y >= cT.wyRangeLow - cT.yRangeLow;
            y--)
        {
            for(int x = cT.wxRangeLow - cT.xRangeLow; 
                    x <= (cT.wxRangeLow - cT.xRangeLow) + (cT.wxRangeHigh - cT.wxRangeLow);
                    x++)
            {

                if(0 <= y && y <= cT.yRangeHigh-cT.yRangeLow &&
                   0 <= x && x <= cT.xRangeHigh-cT.xRangeLow)
                {
                    out << (cT.cells[y][x] ? "1":"~");
                }
                else
                {
                    out << "~";
                }
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
    wxRangeSet = false;
    wyRangeSet = false;
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

void terrain::setWYRange(range_t wyRange)
{
    wyRangeLow = wyRange.low;
    wyRangeHigh = wyRange.high;
    wyRangeSet = true;
}

void terrain::setWXRange(range_t wxRange)
{
    wxRangeLow = wxRange.low;
    wxRangeHigh = wxRange.high;
    wxRangeSet = true;
}

void terrain::setPrintModeAut(bool _printAut)
{
    printAut = _printAut;
}

void terrain::simulate(int cycles)
{
    while(cycles > 0)
    {
        vector< vector<cell> > tmpCells(cells);
        for(int y = 0; y <= yRangeHigh-yRangeLow; y++)
        {
            for(int x = 0; x <= xRangeHigh-xRangeLow; x++)
            {
                tmpCells[y-yRangeLow][x-xRangeLow] = getNextState(x-xRangeLow,y-yRangeLow);
            }
        }
        cycles--;
        cells.assign(tmpCells.begin(), tmpCells.end());
    }
}

int terrain::numberOfLiveNeighbors(int x, int y)
{
    int liveNeighbors = 0;
    for(int i = y-1; i <= y+1; i++)
    {
        for(int j = x-1; j <= x+1; j++)
        {
            if(0 <= i && i <= yRangeHigh-yRangeLow &&
               0 <= j && j <= xRangeHigh-xRangeLow)
            {
                //The cell can't be it's own neighbor
                if(i != y && x != j)
                {
                    liveNeighbors += cells[i][j];
                }
            }
        }
    }

    return liveNeighbors;
}

cell terrain::getNextState(int x, int y)
{
    int liveNeighbors = numberOfLiveNeighbors(x, y);

    cell returnState;

    if(cells[y][x] == ALIVE)
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

    return returnState;
}
