#include <QString>
#include <QtGui>
#include "optionsdialog.h"
#include <sstream>

optionsDialog::optionsDialog(QWidget *parent, std::string name)
    : QDialog(parent)
{
    /*
    label = new QLabel(tr("Find &what:"));
    lineEdit = new QLineEdit;
    label->setBuddy(lineEdit);

    caseCheckBox = new QCheckBox(tr("Match &case"));
    backwardCheckBox = new QCheckBox(tr("Search &backward"));

    findButton = new QPushButton(tr("&Find"));
    findButton->setDefault(true);
    findButton->setEnabled(false);

    closeButton = new QPushButton(tr("Close"));

    connect(lineEdit, SIGNAL(textChanged(const QString&)),
            this, SLOT(enableFindButton(const QString&)));
    connect(findButton, SIGNAL(clicked()),
            this, SLOT(findClicked()));
    connect(closeButton, SIGNAL(clicked()),
            this, SLOT(close()));
            */

    zoomLabel = new QLabel(tr("Zoom factor"));
    delayLabel = new QLabel(tr("Delay factor"));
    generationLabel = new QLabel(tr("Generation:"));
    numberLabel = new QLabel(tr("0"));

    zoomSB = new QSpinBox;
    zoomSB->setRange(1,30);
    delaySB = new QSpinBox;
    delaySB->setRange(1,10000);

    zoomSlid = new QSlider(Qt::Horizontal);
    zoomSlid->setRange(1,30);
    QObject::connect(zoomSB, SIGNAL(valueChanged(int)),
                     zoomSlid, SLOT(setValue(int)));
    QObject::connect(zoomSlid, SIGNAL(valueChanged(int)),
                     zoomSB, SLOT(setValue(int)));
    QObject::connect(zoomSB, SIGNAL(valueChanged(int)),
                     parent, SLOT(zoomChanged(int)));
    zoomSB->setValue(10);
    

    delaySlid = new QSlider(Qt::Horizontal);
    delaySlid->setRange(1,10000);
    QObject::connect(delaySB, SIGNAL(valueChanged(int)),
                     delaySlid, SLOT(setValue(int)));
    QObject::connect(delaySlid, SIGNAL(valueChanged(int)),
                     delaySB, SLOT(setValue(int)));
    QObject::connect(delaySB, SIGNAL(valueChanged(int)),
                     parent, SLOT(delayChanged(int)));
    delaySB->setValue(100);

    quitButton = new QPushButton(tr("&Quit"));
    playButton = new QPushButton(tr("&Play"));
    stepButton = new QPushButton(tr("&Step"));
    QObject::connect(quitButton, SIGNAL(clicked()),
            parent, SLOT(quitClicked()));
    QObject::connect(stepButton, SIGNAL(clicked()),
            parent, SLOT(stepClicked()));
    QObject::connect(playButton, SIGNAL(clicked()),
            parent, SLOT(playClicked()));


    QHBoxLayout *zoomLayout = new QHBoxLayout;
    zoomLayout->addWidget(zoomLabel);
    zoomLayout->addWidget(zoomSB);
    zoomLayout->addWidget(zoomSlid);

    QHBoxLayout *delayLayout = new QHBoxLayout;
    delayLayout->addWidget(delayLabel);
    delayLayout->addWidget(delaySB);
    delayLayout->addWidget(delaySlid);

    QHBoxLayout *generationLayout = new QHBoxLayout;
    generationLayout->addWidget(generationLabel);
    generationLayout->addStretch();
    generationLayout->addWidget(numberLabel);
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(quitButton);
    buttonLayout->addWidget(playButton);
    buttonLayout->addWidget(stepButton);
    
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(zoomLayout);
    mainLayout->addLayout(delayLayout);
    mainLayout->addLayout(generationLayout);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    QString *title = new QString((name + " Controls").c_str());
    setWindowTitle(*title);
    setFixedHeight(sizeHint().height());
}

void optionsDialog::setGen(int g)
{
    std::ostringstream convert;
    convert << g;
    numberLabel->setText(convert.str().c_str());
}

    /*
void optionsDialog::findClicked()
{
    //TODO: delete this
    QString text = lineEdit->text();
    Qt::CaseSensitivity cs = 
        caseCheckBox->isChecked() ? Qt::CaseSensitive
        : Qt::CaseInsensitive;
    if(backwardCheckBox->isChecked())
    {
        emit findPrevious(text, cs);
    }
    else
    {
        emit findNext(text, cs);
    } 
}

void optionsDialog::enableFindButton(const QString &text)
{
    findButton->setEnabled(!text.isEmpty());
}
    */
