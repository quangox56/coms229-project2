//**********************************************************
//  terrain.cpp
//  
//  Author: Ryan Scheel (rascheel@iastate.edu)
//
//  Main source code for the project. This contains the code
//  for the aut parsing, terrain holding, and parsing.
//  Refer to terrain.h for function comments.
//
//**********************************************************
#include"terrain.h"
#include"conwaysGOL.h"
#include"briansBrain.h"
#include"wireWorld.h"
#include"langtonsAnts.h"

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
        while(autFileStatements[i][0] == ' '  || 
              autFileStatements[i][0] == '\n' || 
              autFileStatements[i][0] == '\t' || 
              autFileStatements[i][0] == '\v' || 
              autFileStatements[i][0] == '\r' || 
              autFileStatements[i][0] == '\f')
        {
            autFileStatements[i] = autFileStatements[i].substr(1);
        }
    }

    string keywords[NUM_KEYWORDS] = KEYWORDS;
    //use an istringstream to parse the statements with whitespace delimiting
    istringstream* iss = new istringstream(autFileStatements[0]);
    for(int i = 0; i < semicolonCount; i++)
    {
        string keyword;
        bool keywordFound = false;
        (*iss) >> keyword;
        //loop through all valid keywords and compare to see if this is a valid keyword
        for(int j = 0; j < NUM_KEYWORDS; j++)
        {
            found = 0;
            if((found = keyword.find(keywords[j])) != string::npos)
            {
                if(found == 0)//Keyword must be at the start of the statement
                {
                    if(keywords[j] == "Initial")//Initial requires special handling
                    {
                        delete iss;
                        string cmpdStatement = autFileStatements[i];
                        //If the statement didn't have the closing bracket
                        if(autFileStatements[i].find("}") == string::npos)
                        {
                            //add all statements until we find the closing bracket
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
                        keyword = "Initial";
                        iss = new istringstream(cmpdStatement);
                    }
                    else if(keywords[j] == "Name")//Name requires special handling because
                    {                             //they can have semicolons inside them
                        delete iss;
                        string cmpdStatement = autFileStatements[i];
                        int firstQuote = autFileStatements[i].find("\"");

                        //If the first statement didn't have a closing quote
                        if(autFileStatements[i].find("\"", firstQuote+1) == string::npos) 
                        {
                            i++;
                            //Add all statements until we find the closing quote, or reach
                            //end of statements
                            while(autFileStatements[i].find("\"") == string::npos)
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
                        }
                        //Strip the keyword from the statement
                        cmpdStatement = cmpdStatement.substr(keywords[j].length());
                        keyword = "Name";
                        iss = new istringstream(cmpdStatement);
                    }
                    keywordFound = true;
                    cT.keywordsFound[j] = true;
                }
            }
        }
        if(keywordFound)
        {
            //If a keyword was found then call the helper function
            cT.handleKeyword(*iss, keyword);
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
        if(!cT.keywordsFound[i])
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
        out << "Rules " << cT.sim->name << ";" << endl;

        if(cT.name.length() > 0)
        {
            out << "Name \"" << cT.name << "\";" << endl;
        }

        out << "Chars ";
        for(int i = 0; i < cT.sim->numStates; i++)
        {
            if(i != 0)
            {
                out << ',';
            }
            out << (int)cT.stateChars[i];
        }
        out << ';' << endl;

        out << "Colors ";
        for(int i = 0; i < cT.sim->numStates; i++)
        {
            if(i != 0)
            {
                out << ',';
            }
            out << "(" << cT.stateColors[i].r 
                << "," << cT.stateColors[i].g 
                << "," << cT.stateColors[i].b << ")";
        }
        out << ';' << endl;
        
        out << "Initial {" << endl;

        cell curState = STATE1;

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
                    if(cT.cells[y][x] == curState)
                    {
                        if(firstX)
                        {
                            out << "Y = " << y+cT.yRangeLow << " : ";
                            out << x+cT.xRangeLow;
                            firstX = false;
                        }
                        else
                        {
                            out << "," << x+cT.xRangeLow;
                        }
                    }
                    else if(cT.cells[y][x] != STATE0)
                    {
                        curState = cT.cells[y][x];
                        if(!firstX)
                        {
                            out << ';' << endl;
                        }
                        out << "State " << (int)curState << ";" << endl;
                        out << "Y = " << y+cT.yRangeLow << " : ";
                        firstX = false;
                        out << x+cT.xRangeLow;
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
                    out << cT.stateChars[cT.cells[y][x]];
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
    //This function is large, but it is split nicely into sections by this first
    //if statement depending on the keyword input.
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
        //Check that Xrange and Yrange were processed already.
        for(int i = 0; i < REQ_KEYWORDS-1; i++)
        {
            if(!keywordsFound[i])
            {
                exitWithErr("Missing required keywords before Initial statement.");
            }
        }
        
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

                cell curState = STATE1;//default to state1
                bool foundRightBracket = false;
                
                while(1) //This loop's condition is checked at the end
                {        //with the final if statement
                    char stateStr[5];
                    while(1)
                    {
                        if(!(iss >> stateStr[0]))
                        {
                            exitWithErr("There was an error in the Initial statement.");
                        }

                        //If the first character was a capital S then it might be a state
                        //command. If it's not a state command then it's a malformed file.
                        if(stateStr[0] == 'S')
                        {
                            for(int i = 1; i < 5; i++)
                            {
                                if(!(iss >> stateStr[i]))
                                {
                                    exitWithErr("There was an error in the Initial statement.");
                                }
                            }
                            if(stateStr[0] == 'S' &&
                                    stateStr[1] == 't' &&
                                    stateStr[2] == 'a' &&
                                    stateStr[3] == 't' &&
                                    stateStr[4] == 'e')
                            {
                                int state = -1;
                                char semicolon;
                                if(!(iss >> state) ||
                                        !(iss >> semicolon))
                                {
                                    exitWithErr("There was an error in the Initial statement.");
                                }
                                if(semicolon != ';')
                                {
                                    exitWithErr("There was an error in the Initial statement.");
                                }

                                if(0 <= state && state < sim->numStates)
                                {
                                    curState = (cell)state;
                                }
                                else
                                {
                                    exitWithErr("State declared that is outside rules scope.");
                                }
                            }
                            else
                            {
                                exitWithErr("There was an error in the Initial statement.");
                            }
                        }
                        else//else if it wan't an S then put it back and continue with
                        {   //normal parsing

                            //if it was a right bracket then we had a state declararation
                            //right before the end bracket so we need to break out of the
                            //outer loop
                            if(stateStr[0] == '}')
                            {
                                foundRightBracket = true;
                            }
                            else
                            {
                                iss.unget();
                            }
                            break;
                        }
                    }
                    if(foundRightBracket)
                    {
                        break;
                    }

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

                            //If it fits inside the range then store it
                            //otherwise ignore
                            if(y >=yRangeLow && y <= yRangeHigh &&
                                    x >=xRangeLow && x <= xRangeHigh)
                            {
                                cells[y-yRangeLow][x-xRangeLow] = curState;
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

            //Check to make sure the ascii value is in the range of 256
            if(asciiHolder >= 0 && asciiHolder <= 255)
            {
                if(charIndex < sim->numStates)//max number of states is determined by simulation rules
                {
                    stateChars[charIndex] = asciiHolder;
                }
                else
                {
                    cerr << "Warning: this aut file has more chars than states." << endl;
                    comma = ';';
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
        char readChar;
        if(!(iss >> readChar))
        {
            exitWithErr("There was an error in the Name statement.");
        }
        if(readChar != '"')
        {
            exitWithErr("There was an error in the Name statement.");
        }

        //Have to read the name one character at a time because it can 
        //have semicolons and spaces and other wierd things
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
                exitWithErr("Color values must range from 0 to 255.");
            }

            //Add the color we just read to the stored colors variable
            if(colorIndex < sim->numStates)//Max amount of colors we have to deal with is determined by simulation type
            {
                stateColors[colorIndex] = colorToRead;
                colorIndex++;
            }
            else
            {
                cerr << "Warning: this aut file has more colors than states." << endl;
                readChar = ';';
                break;
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
    else if(keyword == "Rules")
    {
        if(keywordsFound[2] || keywordsFound[4] || keywordsFound[5])
        {
            exitWithErr("The rules keyword must come before Chars, Colors, and Initial keywords.");
        }

        string rules;
        if(!(iss >> rules))
        {
           exitWithErr("There was an error in the Rules statement.");
        }

        //if the rules keyword is immediately followed by a semicolon
        //then that semicolon will accompany it in the read so we
        //need to strip that.
        if(rules.find(";", 0) != string::npos)
        {
            rules = rules.substr(0, rules.length()-1);
        }
        else
        {//else read and make sure the next thing is a semicolon.
            char semicolon;
            if(!(iss >> semicolon))
            {
                exitWithErr("There was an error in the Rules statement.");
            }
            if(semicolon != ';')
            {
                exitWithErr("There was an error in the Rules statement.");
            }
        }

        delete sim;
        if(rules == "ConwaysLife")
        {
            sim = new conwaysGOL(*this);
        }
        else if(rules == "BriansBrain")
        {
            sim = new briansBrain(*this);
        }
        else if(rules == "WireWorld")
        {
            sim = new wireWorld(*this);
        }
        else if(rules == "LangtonsAnt")
        {
            sim = new langtonsAnts(*this);
        }
        else
        {//If they specified an invalid rule.
           exitWithErr("There was an error in the Rules statement.");
        }
    }
    else
    {
        //if we got here that means the helper function got something like
        //Name; as input which is an invalid use of the Name keyword.
        
        //Check which keyword was meant to be passed and print a useful error
        //specifying that keyword.
        string keywords[NUM_KEYWORDS] = KEYWORDS;
        for(int j = 0; j < NUM_KEYWORDS; j++)
        {
            if(keyword.find(keywords[j]) != string::npos)
            {
                exitWithErr("There was an error in the " + keywords[j] + " statement.");
            }
        }
    }
}

terrain::terrain()
{

    sim = new conwaysGOL(*this);//Default rules is conways

    isValid = false;
    printAut = false;
    xRangeSet = false;
    yRangeSet = false;
    wxRangeSet = false;
    wyRangeSet = false;
    stateChars.resize(10);//There is a max of 10 states with langton's ants
    stateColors.resize(10);

    //Initialize the keywords found array to false
    for(int i = 0; i < NUM_KEYWORDS; i++)
    {
        keywordsFound[i] = false;
    }

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

terrain::~terrain()
{
    delete sim;
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
    if(isValid)
    {
        sim->simulate(cycles);
    }
    else
    {
        cerr << "Warning: attempted to simulate before loading terrain." << endl;
    }
}

neighbors_t terrain::neighborInfo(int x, int y)
{
    neighbors_t nearby;
    //initialize neighbors to 0
    for(int i = 0; i < 10; i ++)
    {
        nearby.neighbors[i] = 0;
    }

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
                    nearby.neighbors[cells[i][j]] += 1;
                }
            }
        }
    }

    return nearby;
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

range_t terrain::getWXRange()
{
    range_t returnRange;
    returnRange.high = wxRangeHigh;
    returnRange.low = wxRangeLow;
    return returnRange;
}

range_t terrain::getWYRange()
{
    range_t returnRange;
    returnRange.high = wyRangeHigh;
    returnRange.low = wyRangeLow;
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

terrain::simulator::simulator(terrain& owner) : t(owner) {}
