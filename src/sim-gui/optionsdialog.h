#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <string>

class QLabel;
class QPushButon;
class QButton;
class QSlider;
class QSpinBox;

class optionsDialog : public QDialog
{
    Q_OBJECT

    public:
        optionsDialog(QWidget *parent =0, std::string name="");
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
