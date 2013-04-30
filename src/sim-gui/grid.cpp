//**********************************************************
//  grid.cpp
//  
//  Author: Ryan Scheel (rascheel@iastate.edu)
//
//  Contains the implementation code for the grid GUI component.
//  Refer to header file grid.h for function comments.
//
//**********************************************************
#include <QtGui>
#include <QTimer>
#include "grid.h"
#include<iostream>
#include<cstdlib>


grid::grid(terrain &t, QWidget *parent) : QWidget(parent), terra(t)
{
    setAttribute(Qt::WA_StaticContents);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()),
                     this, SLOT(stepClicked()));

    generation = 0;
    
    zoom = 1;
    t.setWXRange(t.getXRange());
    t.setWYRange(t.getYRange());

    xRange = t.getWXRange();
    yRange = t.getWYRange();
    image = QImage(xRange.high-xRange.low+1, yRange.high-yRange.low+1, QImage::Format_ARGB32);
    updateImage();

    QString *title = new QString(t.getName().c_str());
    setWindowTitle(*title);

    optionsD = new optionsDialog(this, t.getName());
    optionsD->show();
    optionsD->activateWindow();
}

void grid::updateImage()
{
    for(int y = 0; y < (yRange.high-yRange.low+1)*zoom; y+=zoom)
    {
        for(int x = 0; x < (xRange.high-xRange.low+1)*zoom; x+=zoom)
        {
            QPoint pos(x, y);
            color_t color = terra.getWindowStateColor(x/zoom, y/zoom);
            setImagePixel(pos, color);
        }
    }
    update();
}

QSize grid::sizeHint() const
{
    QSize size = zoom * image.size();
    if(zoom >= 3)
        size += QSize(1,1);
    return size;
}

void grid::setZoomFactor(int newZoom)
{
    if(newZoom < 1)
        newZoom = 1;

    if(newZoom != zoom)
    {
        zoom = newZoom;
        update();
        updateGeometry();
        resize(sizeHint());
    }
}

void grid::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if(zoom >= 3)
    {
        painter.setPen(palette().foreground().color());
        for(int i = 0; i <= image.width(); ++i)
            painter.drawLine(zoom*i, 0, zoom*i, zoom*image.height());
        for(int j = 0; j <= image.height(); ++j)
            painter.drawLine(0, zoom*j, zoom*image.width(), zoom*j);
    }
    for(int i = 0; i < image.width(); ++i)
    {
        for(int j = 0; j < image.height(); ++j)
        {
            QRect rect = pixelRect(i, j);
            if (!event->region().intersect(rect).isEmpty())
            {
                QColor color = QColor::fromRgba(image.pixel(i, j));
                painter.fillRect(rect, color);
            }
        }
    }
}

QRect grid::pixelRect(int i, int j) const
{
    if(zoom >= 3)
    {
        return QRect(zoom*i, zoom *j+1, zoom - 1, zoom - 1);
    }
    else
    {
        return QRect(zoom*i, zoom*j, zoom, zoom);
    }
}

void grid::setImagePixel(const QPoint &pos, color_t color)
{
    int i = pos.x() / zoom;
    int j = pos.y() / zoom;

    if (image.rect().contains(i,j))
    {
        image.setPixel(i,j, qRgb(color.r, color.g, color.b));
    }
}

void grid::stepClicked()
{
    terra.simulate(1);
    optionsD->setGen(++generation);
    updateImage();
}

void grid::playClicked()
{
    if(timer->isActive())
    {
        timer->stop();
    }
    else
    {
        timer->start(delay);
    }
}

void grid::quitClicked()
{
    exit(0);
}

void grid::zoomChanged(int z)
{
    setZoomFactor(z);
}

void grid::delayChanged(int d)
{
    delay = d;
    if(timer->isActive())
    {
        timer->stop();
        timer->start(delay);
    }
}
