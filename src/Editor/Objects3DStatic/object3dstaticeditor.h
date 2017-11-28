#ifndef OBJECTSTATIC3DEDITOR_H
#define OBJECTSTATIC3DEDITOR_H

#include <QMainWindow>
#include <QPushButton>

class MatButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MatButton( QWidget *parent = 0): QPushButton(parent){

    }

    void mouseReleaseEvent(QMouseEvent *e){
        emit clicked(this);
    }



    int id;
signals:
    void clicked(MatButton*);
};

namespace Ui {
    class Object3DStaticEditor;
}


class Object3DStaticEditor : public QMainWindow
{
    Q_OBJECT
public:
    explicit Object3DStaticEditor( QWidget *parent = 0);
    void init(QString path="");
    ~Object3DStaticEditor();

    void closeEvent(QCloseEvent *event);

public slots:
    void setMaterial(MatButton *button);
    void saveObject();

private:
    Ui::Object3DStaticEditor *ui;
    unsigned int m_handleObject;
    QString m_path;
    unsigned int m_models_count;
    std::vector<QString>* m_materialsPath;

};

#endif // OBJECTSTATIC3DEDITOR_H
