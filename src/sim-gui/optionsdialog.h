#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButon;

class optionsDialog : public QDialog
{
    Q_OBJECT

    public:
        optionsDialog(QWidget *parent =0);

    signals:
        void findNext(const QString &str, Qt::CaseSensitivity cs);
        void findPrevious(const QString &str, Qt::CaseSensitivity cs);

        private slots:
            void findClicked();
            void enableFindButton(const QString &text);

        private:
            QLabel *label;
            QLineEdit *lineEdit;
            QCheckBox *caseCheckBox;
            QCheckBox *backwardCheckBox;
            QPushButton *findButton;
            QPushButton *closeButton;
};

#endif
