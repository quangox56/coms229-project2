#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButon;

class FindDialog : public QDialog
{
    Q_OBJECT

    public:
        FindDialog(QWidget *paretn =0);

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
