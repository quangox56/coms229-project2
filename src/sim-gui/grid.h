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
    Q_PROPERTY(QColor penColor READ penColor WRITE setPenColor)
    Q_PROPERTY(QImage iconImage READ iconImage WRITE setIconImage)
    Q_PROPERTY(int zoomFactor READ zoomFactor WRITE setZoomFactor)

    public:
    grid(terrain& t, QWidget *parent = 0);
    void updateImage();

    void setPenColor(const QColor &newColor);
    QColor penColor() const { return curColor; }

    void setZoomFactor(int newZoom);
    int zoomFactor() const { return zoom; }
    void setIconImage(const QImage &newImage);
    QImage iconImage() const { return image;}
    QSize sizeHint() const;

    public slots:
        void playClicked();
        void stepClicked();
        void quitClicked();
        void zoomChanged(int);
        void delayChanged(int);

    protected:
    //void mousePressEvent(QMouseEvent *event);
    //void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

    private:
    void setImagePixel(const QPoint &pos, color_t color);
    QRect pixelRect(int i, int j) const;

    terrain &terra;
    QColor curColor;
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
