#include<iostream>
#include<string>

using namespace std;

istream& terrain::operator>>(istream& in, terrain& cTerrain)
{
    string autFille;

    string tmp;
    while(in >> tmp)//Put the entire aut file into a string
    {
        autFile += tmp;
    }

    int lineCount = 1;//The file obviously starts with one line
    unsigned found = 0;
    //count the number of lines
    while(found = autFile.find("\n", found) != string::npos)
    {
        lineCount++;
    }

    //This string will hold each line. This is necessary to simplify comments.
    string* autFileLines = malloc(sizeof(string)*lineCount);

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
    //Find comment symbol # and remove all comments.
    //Also count all semicolons while looping through all the lines.
    for(int i = 0; i < lineCount; i++)
    {
        found = autFileLines[i].find("#");
        if(found != string::npos)
        {
            autFileLines[i] = autFileLines[i].substr(0, found);
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

    istringstream iss = new istringstream(autFileStatements[0]);
    string keywords[NUM_KEYWORDS] = KEYWORDS;
    bool keywordsFound[NUM_KEYWORDS];
    for(int i = 0; i < semicolonCount; i++)
    {
        string tmp;
        bool keywordFound = false;
        iss >> tmp;
        for(int i = 0; i < NUM_KEYWORDS; i++)
        {
            if(tmp == keywords[i])
            {
                keywordFound = true;
                keywordsFound[i] = true;
            }
        }
        string keyword;
        if(keywordFound)
        {
            keyword = tmp;

        }
    }



}

void handleKeyword(istringstream&, string keyword)
{
    switch(keyword)
    {
        case "Xrange":
            if((iss >> xRangeLow || iss >> xRangeHigh))
            {
                cerr << "There was an error in the xRange statement." << endl;
                cerr << "Program is now exiting with error." << endl;
                exit(1);
            }
        case "Yrange":
            if((iss >> yRangeLow || iss >> yRangeHigh))
            {
                cerr << "There was an error in the yRange statement." << endl;
                cerr << "Program is now exiting with error." << endl;
                exit(1);
            }
        case "Initial":

    }

}



