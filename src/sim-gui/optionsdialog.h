#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <string>

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButon;
class QButton;
class QSlider;
class QSpinBox;

class optionsDialog : public QDialog
{
    Q_OBJECT

    public:
        optionsDialog(QWidget *parent =0, std::string name="");

    signals:
        void findNext(const QString &str, Qt::CaseSensitivity cs);
        void findPrevious(const QString &str, Qt::CaseSensitivity cs);

        //private slots:
            //void findClicked();
            //void enableFindButton(const QString &text);

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
