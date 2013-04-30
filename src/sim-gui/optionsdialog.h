//**********************************************************
//  optionsdialog.h
//  
//  Author: Ryan Scheel (rascheel@iastate.edu)
//
//  Contains the declarations for the optionsDialog class.
//
//**********************************************************
#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <string>

class QLabel;
class QPushButon;
class QButton;
class QSlider;
class QSpinBox;

/* Class: optionsDialog
 *
 * This class has the GUI logic for the sim-gui options dialog.
 * It contains a place to change the delay time and zoom factor,
 * and buttons to quit, step by on through the sim, or play the sim
 * continuously.
 */
class optionsDialog : public QDialog
{
    Q_OBJECT

    public:
        /* Function: optionsDialog
         *
         * Constructor for optionsDialog. Initializes all the widgets
         * used inside of opionsDialog and sets their relationships to eachother.
         */
        optionsDialog(QWidget *parent =0, std::string name="");
        /* Function: setGen
         *
         * This function is called by the parent to update the numbe of generations
         * to display.
         */
        void setGen(int g);


    private:
        QSpinBox *zoomSB;
        QSpinBox *delaySB;
        QSlider *zoomSlid;
        QSlider *delaySlid;
        QLabel *zoomLabel;
        QLabel *delayLabel;
        QLabel *generationLabel;
        QLabel *numberLabel;
        QPushButton *quitButton;
        QPushButton *playButton;
        QPushButton *stepButton;
};

#endif
