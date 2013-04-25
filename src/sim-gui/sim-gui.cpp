#include <QApplication>
#include <QHBoxLayout>
#include <QSlider>
#include <QSpinBox>
#include <QPushButton>
#include "grid.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iterator>
#include <ncurses.h>
#include"../terrain.h"
#include"../common.h"

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

int main(int argc, char *argv[])
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


    /*
    QApplication app(argc, argv);
    QWidget *window = new QWidget;
    window->setWindowTitle("Enter Your Age");

    QSpinBox *spinBox = new QSpinBox;
    QSlider *slider = new QSlider(Qt::Horizontal);
    spinBox->setRange(0,130);
    slider->setRange(0,130);

    QObject::connect(spinBox, SIGNAL(valueChanged(int)),
                     slider, SLOT(setValue(int)));
    QObject::connect(slider, SIGNAL(valueChanged(int)),
                     spinBox, SLOT(setValue(int)));
    spinBox->setValue(35);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(spinBox);
    layout->addWidget(slider);
    window->setLayout(layout);
    window->show();

    //QPushButton *button = new QPushButton("Quit");
    //QObject::connect(button, SIGNAL(clicked()), &app, SLOT(quit()));
    //button->show();
    return app.exec();
    */
    QApplication app(argc, argv);
    //optionsDialog *dialog = new optionsDialog;
    grid *dialog = new grid;
    dialog->show();

//    optionsDialog *dialog2 = new optionsDialog(dialog);
//    dialog2->show();
//    dialog2->activateWindow();

    return app.exec();
}
