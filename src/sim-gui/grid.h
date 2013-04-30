//**********************************************************
//  grid.h
//  
//  Author: Ryan Scheel (rascheel@iastate.edu)
//
//  Header for the grid class.
//
//**********************************************************
#ifndef GRID_H
#define GRID_H

#include <QColor>
#include <QImage>
#include <QWidget>

#include "../common.h"
#include "../terrain.h"
#include <string>
#include "optionsdialog.h"

/* Class: grid
 *
 * This class contains all the Qt code for launching the grid.
 * It makes use of the terrain class to do the cellular
 * automaton simulation.
 */
class grid: public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int zoomFactor READ zoomFactor WRITE setZoomFactor)

    public:
        /* Function: grid
         *
         * Constructor for the class. Sets up the grid and initializes
         * the child optionsDialog.
         */
        grid(terrain& t, QWidget *parent = 0);

        /* Function: updateImage
         *
         * This function is called to update the image. It gets
         * the cell status from terra and then calls the necessary
         * draw funcions.
         */
        void updateImage();


        /* Function: setZoomFactor
         *
         * This function sets and updates the zoom factor.
         */
        void setZoomFactor(int newZoom);

        /* Function: zoomFactor
         *
         * This function returns the current zoom size.
         */
        int zoomFactor() const { return zoom; }


        /* Function: sizeHine
         *
         * Calculates the recommended size for the grid. Qt
         * uses this to know how big it should size the initial window.
         */
        QSize sizeHint() const;

    public slots:
        /* Function: playClicked
         *
         * This function is called whenever the play button is 
         * pressed inside the options dialog. If the simulation
         * is already running then it will pause, otherwise it
         * starts the sim with the current delay.
         */
        void playClicked();
        /* Function: stepClicked
         *
         * This function is called whenever the step button is
         * pressed in the options dialog. It runs through a single
         * simulation.
         */
        void stepClicked();
        /* Function: quitClicked
         *
         * This is called when the quit button is called, it 
         * exits the application.
         */
        void quitClicked();
        /* Function: zoomChanged
         *
         * This is called whenever the zoom changes in the option
         * dialog. It updates the new zoom value and redraws.
         */
        void zoomChanged(int);
        /* Function: delayChanged
         *
         * When the delay is changed this function is called.
         * it updates the delay value and if we were in the middle
         * of simulating it will stop the QTimer and restart it 
         * with the updated delay value.
         */
        void delayChanged(int);
    protected:
        /* Function: paintEvent
         *
         * This function is called when the widget recieves a request
         * to repaint all or part of itself. Usually by the update()
         * function.
         */
        void paintEvent(QPaintEvent *event);

    private:
        /* Function: setImagePixel
         *
         * This funcion sets the new color for an individual cell after
         * a simulation step.
         */
        void setImagePixel(const QPoint &pos, color_t color);
        /* Function: pixelRect
         *
         * Returns a rectangle based on he coordinates passed and zoom 
         * level.
         */
        QRect pixelRect(int i, int j) const;

        range_t xRange;
        range_t yRange;
        terrain &terra;
        QImage image;
        int zoom;
        int generation;
        int delay;
        range_t wxRange;
        range_t wyRange;
        QTimer *timer;

        optionsDialog *optionsD;
};

#endif
