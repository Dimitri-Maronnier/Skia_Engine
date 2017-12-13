#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>

#include <QMainWindow>
#include <GL/glew.h>
#include <QGLWidget>
#include <QGLContext>
#include "Editor/scenetree.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();
    SceneTree *getSceneTree();
    static QGLContext *contxt;

public slots:
    void newProject();
    void openProject();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
