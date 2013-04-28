#ifndef GRID_H
#define GRID_H

#include <QColor>
#include <QImage>
#include <QWidget>

#include "../common.h"
#include "../terrain.h"
#include <string>
#include "optionsdialog.h"

class grid: public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int zoomFactor READ zoomFactor WRITE setZoomFactor)

    public:
    grid(terrain& t, QWidget *parent = 0);
    void updateImage();


    void setZoomFactor(int newZoom);
    int zoomFactor() const { return zoom; }
    QSize sizeHint() const;

    public slots:
        void playClicked();
        void stepClicked();
        void quitClicked();
        void zoomChanged(int);
        void delayChanged(int);

    protected:
    void paintEvent(QPaintEvent *event);

    private:
    void setImagePixel(const QPoint &pos, color_t color);
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
