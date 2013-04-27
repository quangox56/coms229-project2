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
    string* autFileStatements = new string[semicolonCount+1];
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
        while(autFileStatements[i][0] == ' ' || 
              autFileStatements[i][0] == '\n' || 
              autFileStatements[i][0] == '\t' || 
              autFileStatements[i][0] == '\v' || 
              autFileStatements[i][0] == '\r' || 
              autFileStatements[i][0] == '\f')
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
                    if(keywords[j] == "Initial")//Initial requires special handling
                    {
                        delete iss;
                        string cmpdStatement = autFileStatements[i];
                        if(autFileStatements[i].find("}") == string::npos)
                        {
                            while(autFileStatements[i].find("}") == string::npos)
                            {
                                if(i < semicolonCount)
                                {
                                    i++;
                                    cmpdStatement += autFileStatements[i];
                                }
                                else
                                {
                                    break;
                                }
                            }
                            i++;
                            if(i < semicolonCount)
                            {
                                cmpdStatement += autFileStatements[i];
                            }
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
    for(int i = 0; i < REQ_KEYWORDS; i++)
    {
        if(!keywordsFound[i])
        {
            exitWithErr("Error: Not all required keywords found in .aut file.");
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
                            out << "Y = " << y << " : ";
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
            if(!firstX)
            {

                out << ";" << endl;
            }
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
                    out << (cT.cells[y][x] ? cT.stateChars[1]:cT.stateChars[0]);
                }
                else
                {
                    out << cT.stateChars[0];
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
                exitWithErr("There was an error in the xRange statement.");
            }
            if(semicolon != ';')
            {
                exitWithErr("There was an error in the xRange statement.");
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
                exitWithErr("There was an error in the yRange statement.");
            }
            if(semicolon != ';')
            {
                exitWithErr("There was an error in the yRange statement.");
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
            exitWithErr("There was an error in the Initial statement.");
        }
        else
        {
            if(leftBracket != '{')
            {
                exitWithErr("There was an error in the Initial statement.");
            }
        }

        if(!(iss >> rightBracket))
        {
            exitWithErr("There was an error in the Initial statement.");
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
                        exitWithErr("There was an error in the Initial statement.");
                    }
                    else
                    {
                        if(Y != 'Y' || equals != '=' || colon != ':')
                        {
                            exitWithErr("There was an error in the Initial statement.");
                        }
                    }

                    //Begin parsing corresponding x-list
                    do
                    {
                        if(!(iss>>x) || !(iss>>comma))
                        {
                            exitWithErr("There was an error in the Initial statement.");
                        }
                        else
                        {
                            if(comma != ',' && comma != ';')
                            {
                                exitWithErr("There was an error in the Initial statement.");
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
                        exitWithErr("There was an error in the Initial statement.");
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
                    exitWithErr("There was an error in the Initial statement.");
                }
                else
                {
                    if(semicolon != ';')
                    {
                        exitWithErr("There was an error in the Initial statement.");
                    }
                }
            }

        }
    }
    else if(keyword == "Chars")
    {
        int charIndex = 0;
        int asciiHolder = 0;

        char comma = ',';
        do
        {
            if(comma != ',')
            {
                exitWithErr("There was an error in the Chars statement.");
            }
            if(!(iss >> asciiHolder) || 
                    !(iss >> comma))
            {
                exitWithErr("There was an error in the Chars statement.");
            }
            if(asciiHolder >= 0 && asciiHolder <= 255)
            {
                if(charIndex < 10)//There is a max of 10 possible states with langston's ants
                {
                    stateChars[charIndex] = asciiHolder;
                }
                else
                {
                    cerr << "Warning: this aut file has more chars than states." << endl;
                }
            }
            else
            {
                exitWithErr("There was an error in the Chars statement.");
            }   
            charIndex++;
        }while(comma != ';');
        
    }
    else if(keyword == "Name")
    {   
        //TODO: Handle the case of a semicolon inside the quotes.
        //This will require some changes inside the operator>> function
        char readChar;
        if(!(iss >> readChar))
        {
            exitWithErr("There was an error in the Name statement.");
        }
        if(readChar != '"')
        {
            exitWithErr("There was an error in the Name statement.");
        }

        do
        {
            if(!(iss >> readChar))
            {
                exitWithErr("There was an error in the Name statement.");
            }
            if(readChar != '"')
            {
                name += readChar;
            }
        }while(readChar != '"');

        if(!(iss >> readChar))
        {
            exitWithErr("There was an error in the Name statement.");
        }

        if(readChar != ';')
        {
            exitWithErr("There was an error in the Name statement.");
        }

    }
    else if(keyword == "Colors")
    {
        char readChar;
        size_t colorIndex = 0;
        color_t colorToRead;

        do
        {
            //Read the first paren enclosing the colors
            if(!(iss >> readChar))
            {
                exitWithErr("There was an error in the Colors statement.");
            }
            if(readChar != '(')
            {
                exitWithErr("There was an error in the Colors statement.");
            }
            //Read the red value
            if(!(iss >> colorToRead.r))
            {
                exitWithErr("There was an error in the Colors statement.");
            }
            //read the comma between red and green
            if(!(iss >> readChar))
            {
                exitWithErr("There was an error in the Colors statement.");
            }
            if(readChar != ',')
            {
                exitWithErr("There was an error in the Colors statement.");
            }
            //read the green value
            if(!(iss >> colorToRead.g))
            {
                exitWithErr("There was an error in the Colors statement.");
            }
            //read the comma between green and blue
            if(!(iss >> readChar))
            {
                exitWithErr("There was an error in the Colors statement.");
            }
            if(readChar != ',')
            {
                exitWithErr("There was an error in the Colors statement.");
            }
            //read the blue value
            if(!(iss >> colorToRead.b))
            {
                exitWithErr("There was an error in the Colors statement.");
            }
            //read the closing paren
            if(!(iss >> readChar))
            {
                exitWithErr("There was an error in the Colors statement.");
            }
            if(readChar != ')')
            {
                exitWithErr("There was an error in the Colors statement.");
            }
            //RGB needs to range from 0 to 255
            if(colorToRead.r < 0 || colorToRead.r > 255 ||
                    colorToRead.g < 0 || colorToRead.g > 255 ||  
                    colorToRead.b < 0 || colorToRead.b > 255)
            {
                exitWithErr("There was an error in the Colors statement.");
            }

            //Add the color we just read to the stored colors variable
            if(colorIndex < 10)//Max amount of colors we have to deal with is 10
            {
                stateColors[colorIndex] = colorToRead;
                colorIndex++;
            }

            //if this char is a comma we have another color value to read
            if(!(iss >> readChar))
            {
                exitWithErr("There was an error in the Colors statement.");
            }
            //else leave the while
        }while(readChar == ',');

        //make sure there is a semicolon at the end.
        if(readChar != ';')
        {
            exitWithErr("There was an error in the Colors statement.");
        }
    }
    else
    {
        cerr << "Shouldnt reach here, unhandled keyword passed.\n";
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
    stateChars.resize(10);//There is a max of 10 states with langton's ants
    stateColors.resize(10);

    //Setting default states
    stateChars[0] = '~';
    stateChars[1] = '1';
    stateChars[2] = '2';
    stateChars[3] = '3';
    stateChars[4] = '4';
    stateChars[5] = '5';
    stateChars[6] = '6';
    stateChars[7] = '7';
    stateChars[8] = '8';
    stateChars[9] = '9';

    //Black
    stateColors[0].r = 0;
    stateColors[0].g = 0;
    stateColors[0].b = 0;

    //White
    stateColors[1].r = 255;
    stateColors[1].g = 255;
    stateColors[1].b = 255;

    //Brown
    stateColors[2].r = 102;
    stateColors[2].g = 51;
    stateColors[2].b = 0;

    //Red
    stateColors[3].r = 255;
    stateColors[3].g = 0;
    stateColors[3].b = 0;

    //Orange
    stateColors[4].r = 255;
    stateColors[4].g = 128;
    stateColors[4].b = 0;

    //Yellow
    stateColors[5].r = 255;
    stateColors[5].g = 255;
    stateColors[5].b = 0;

    //Green
    stateColors[6].r = 0;
    stateColors[6].g = 255;
    stateColors[6].b = 0;

    //Teal
    stateColors[7].r = 0;
    stateColors[7].g = 255;
    stateColors[7].b = 255;

    //Blue
    stateColors[8].r = 0;
    stateColors[8].g = 0;
    stateColors[8].b = 255;

    //Purple
    stateColors[9].r = 111;
    stateColors[9].g = 0;
    stateColors[9].b = 255;
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
        for(int y = 0; 
            y <= (yRangeHigh-yRangeLow);
            y++)
        {
            for(int x = 0; 
                    x <= (xRangeHigh-xRangeLow);
                    x++)
            {
                if(0 <= y && y <= cells.size())
                {
                    if(0 <= x && x <= cells[y].size())
                    {
                        tmpCells[y][x] = getNextState(x,y);
                    }
                }
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
            //Make sure the cell is inside the range
            if(0 <= i && i <= (yRangeHigh-yRangeLow) &&
               0 <= j && j <= (xRangeHigh-xRangeLow))
            {
                //The cell can't be it's own neighbor
                if(!(i == y && x == j))
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

char getWindowState(int x, int y)
{
    //TODO: placeholder, implement this to return the window adjusted coordinates.
    return 'F';
}

string terrain::getName()
{
    return name;
}

range_t terrain::getYRange()
{
    range_t returnRange;
    returnRange.high = yRangeHigh;
    returnRange.low = yRangeLow;
    return returnRange;
}

range_t terrain::getXRange()
{
    range_t returnRange;
    returnRange.high = xRangeHigh;
    returnRange.low = xRangeLow;
    return returnRange;
}

char terrain::getWindowState(int x, int y)
{
    int wY = (wyRangeLow - yRangeLow) + (wyRangeHigh - wyRangeLow) - y;
    int wX = wxRangeLow - xRangeLow + x;
    if(0 <= wY && wY <= yRangeHigh-yRangeLow &&
            0 <= wX && wX <= xRangeHigh-xRangeLow)
    {
        return stateChars[cells[wY][wX]];
    }
    else
    {
        return stateChars[0];
    }
}

color_t terrain::getWindowStateColor(int x, int y)
{
    int wY = (wyRangeLow - yRangeLow) + (wyRangeHigh - wyRangeLow) - y;
    int wX = wxRangeLow - xRangeLow + x;
    if(0 <= wY && wY <= yRangeHigh-yRangeLow &&
            0 <= wX && wX <= xRangeHigh-xRangeLow)
    {
        return stateColors[cells[wY][wX]];
    }
    else
    {
        return stateColors[0];
    }
}
