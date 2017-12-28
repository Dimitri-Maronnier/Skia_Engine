#ifndef NEWACTORDIALOG_H
#define NEWACTORDIALOG_H
#include <QDialog>

namespace Ui {
class NewActorDialog;
}

class NewActorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewActorDialog( QWidget * parent = 0);
    ~NewActorDialog();
public slots:
    void accept();
private:
    Ui::NewActorDialog *ui;
    static QString DefaultHeader;
    static QString DefaultSource;
    static QString DefaultPro;

};
#endif // NEWACTORDIALOG_H
