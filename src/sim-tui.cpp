#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iterator>
#include <ncurses.h>
#include"terrain.h"
#include"common.h"

using namespace std;

struct winParam
{
    int delay;
    range_t wxRange;
    range_t wyRange;
    terrain terra;
    int col;
    int row;
    int currentGen;
    bool playing;
};

struct keys
{
    bool q;
    bool p;
    bool s;
    bool lArr;//left arrow
    bool rArr;//right arrow
    bool uArr;//up arrow
    bool dArr;//down arrow
    bool plus;
    bool minus;
};

void initDisplay(winParam &wp)
{
    initscr();
    keypad(stdscr, TRUE);
    cbreak();
    noecho();
    curs_set(0);//Set cursor invisible
    getmaxyx(stdscr, wp.row, wp.col);
    if(wp.col < 40 || wp.row < 20)
    {
        endwin();
        exitWithErr("Terminal is too small. Minimum column size is 40, minimum row size is 20.");
    }
}

void updateDisplay(winParam &wp)
{
    wp.terra.setWYRange(wp.wyRange);
    wp.terra.setWXRange(wp.wxRange);
    //Print the title.
    mvprintw(0,(wp.col-wp.terra.getName().length())/2,"%s",wp.terra.getName().c_str());

    //Print Delay and Generation info
    mvprintw(1,0, "Delay: %4i (+/-)", wp.delay);
    mvprintw(1,wp.col-16, "Generation %5i", wp.currentGen); 
    //Draw the top border
    move(2,0);
    addch('+');
    for(int i = 1; i < wp.col-1; i++)
    {
        addch('-');
    }
    addch('+');

    //Draw the left border
    for(int i = 3; i < wp.row-2; i++)
    {
        mvaddch(i, 0, '|');
    }
    //Draw bottom border
    move(wp.row-2, 0);
    addch('+');
    addch('<');
    range_t xRange = wp.terra.getXRange();
    double perc = 1.0/(double)(xRange.high-xRange.low+1);
    float openLeft = perc*(float)(wp.wxRange.low-xRange.low);
    float scrollbar = perc*(float)(wp.wxRange.high-wp.wxRange.low);
    float openRight = perc*(float)(xRange.high-wp.wxRange.high);
    for(int i = 2; i < wp.col-2; i++)
    {
        float normalized = (float)i/(float)(wp.col-3);
        //cerr << normalized << " " << openLeft << " " << scrollbar << endl; 
        if(normalized < openLeft)
        {
            addch('-');
        }
        else if(normalized < (scrollbar+openLeft))
        {
            addch('#');
        }
        else
        {
            addch('-');
        }
    }
    addch('>');
    addch('+');

    //Draw right border
    mvaddch(3, wp.col-1, '^');
    range_t yRange = wp.terra.getYRange();
    perc = 1.0/(double)(yRange.high-yRange.low);
    float openBot = perc*(float)(wp.wyRange.low-yRange.low);
    scrollbar = perc*(float)(wp.wyRange.high-wp.wyRange.low);
    float openTop = perc*(float)(yRange.high-wp.wyRange.high);
    for(int i = 4; i < wp.row-3; i++)
    {
        float normalized = (float)i/(float)(wp.row-3);
        //cerr << normalized << " " << openLeft << " " << scrollbar << endl; 
        if(normalized < openTop)
        {
            mvaddch(i, wp.col-1, '|');
        }
        else if(normalized < (scrollbar+openTop))
        {
            mvaddch(i, wp.col-1, '#');
        }
        else
        {
            mvaddch(i, wp.col-1, '|');
        }
    }
    mvaddch(wp.row-3, wp.col-1, 'V');

    
    //Draw keypress info
    mvprintw(wp.row-1, 0, "(Q)uit");
    mvprintw(wp.row-1, (int)((wp.col-1)*(1.0/4.0)-3), "(P)lay");
    mvprintw(wp.row-1, (int)((wp.col-1)*(3.0/4.0)-3), "(S)tep");
    mvprintw(wp.row-1, wp.col-16, "Arrows:scroll");

    //Draw Terrain
    int maxY = wp.row-5;
    int maxX = wp.col-2;
    for(int y = 0; y < maxY; y++)
    {
        move(3+y, 1);
        for(int x = 0; x < maxX; x++)
        {
            addch(wp.terra.getWindowState(x, y));
        }
    }
    refresh();
}

void handleKeys(keys &k, winParam &wp)
{
    timeout(wp.delay);
    int ch = getch();
    k.q = (ch == 'q' || ch == 'Q');
    k.p = (ch == 'p' || ch == 'P');
    k.s = (ch == 's' || ch == 'S');
    k.lArr = (ch == KEY_LEFT);
    k.rArr = (ch == KEY_RIGHT);
    k.uArr = (ch == KEY_UP);
    k.dArr = (ch == KEY_DOWN);
    k.plus = (ch == '+');
    k.minus = (ch == '-');

    if(k.p)
    {
        wp.playing = !wp.playing;
    }
    if(k.plus)
    {
        if(wp.delay < 9990)
        {
            wp.delay = wp.delay + 10;
        }
    }
    if(k.minus)
    {
        if(wp.delay > 10)
        {
            wp.delay = wp.delay - 10;
        }
    }
    if(k.lArr)
    {
        range_t xRange = wp.terra.getXRange();
        if(xRange.low < wp.wxRange.low)
        {
            wp.wxRange.high = wp.wxRange.high - 1;
            wp.wxRange.low = wp.wxRange.low - 1;
        }
    }
    if(k.rArr)
    {
        range_t xRange = wp.terra.getXRange();
        if(xRange.high+2 > wp.wxRange.high)
        {
            wp.wxRange.high = wp.wxRange.high + 1;
            wp.wxRange.low = wp.wxRange.low + 1;
        }
    }
    if(k.uArr)
    {
        range_t yRange = wp.terra.getYRange();
        if(yRange.high > wp.wyRange.high)
        {
            wp.wyRange.high = wp.wyRange.high + 1;
            wp.wyRange.low = wp.wyRange.low + 1;
        }
    }
    if(k.dArr)
    {
        range_t yRange = wp.terra.getYRange();
        if(yRange.low < wp.wyRange.low)
        {
            wp.wyRange.high = wp.wyRange.high - 1;
            wp.wyRange.low = wp.wyRange.low - 1;
        }
    }
}

int main(int argc, char** argv)
{
    vector<string> argvString(0);

    for(int i = 1; i < argc; i++)
    {
        argvString.push_back(argv[i]);
    }

    bool hFlag = optionExists(argvString, "-h");
    bool txFlag = optionExists(argvString, "-tx");
    bool tyFlag = optionExists(argvString, "-ty");

    if(hFlag)
    {
        //TODO: PRINT HELP TEXT
        cout << "help flag" << endl;
        exit(0);
    }

    int generations = 0;
    range_t txRange;
    range_t tyRange;

    winParam wp;//terra is latin for earth :)
    if(txFlag)
    {
        txRange = getRange(argvString,"-tx");
        wp.terra.setXRange(txRange);
    }
    if(tyFlag)
    {
        tyRange = getRange(argvString,"-ty");
        wp.terra.setYRange(tyRange);
    }

    string filename = getInputFileName(argvString);
    if(filename.size() != 0)
    {
        ifstream inputFile;
        inputFile.open(filename.c_str());
        inputFile >> wp.terra;
        inputFile.close();
    }
    else
    {
        cin >> wp.terra;
    }

    initDisplay(wp);

    wp.delay = 100; //Default the delay to 100ms TODO: add keypresses to change delay
    wp.currentGen = 0;
    wp.playing = false;
    range_t xRange = wp.terra.getXRange();
    range_t yRange = wp.terra.getYRange();
    wp.wyRange.low = yRange.low;
    wp.wyRange.high = yRange.low + wp.row-6;
    wp.wxRange.low = xRange.low;
    wp.wxRange.high = xRange.low + wp.col-5;


    keys k;
    while(1)
    {
        updateDisplay(wp);
        handleKeys(k, wp);
        if(k.q) break;
        if(k.s || wp.playing) 
        {
            wp.terra.simulate(1);
            wp.currentGen++;
        }
    }
    endwin();
    return 0;
}
