#ifndef MATERIALEDITORWINDOW_H
#define MATERIALEDITORWINDOW_H

#include <QMainWindow>
#include <QTextBrowser>
#include "src/Editor/Materials/Nodes/nodematerial.h"
#include <glm/vec3.hpp>
#include "src/Editor/Materials/materialgraphwidget.h"
#include "src/Editor/Materials/glmaterialpreview.h"

namespace Ui {
    class MaterialEditorWindow;
}

class MaterialEditorWindow: public QMainWindow
{
    Q_OBJECT

public:
    explicit MaterialEditorWindow(QWidget * parent = 0);
    ~MaterialEditorWindow();
    MaterialGraphWidget * getMaterialView();
    GLMaterialPreview* getGlPreview();
    QTextBrowser *getDebugBrowser();
    QString getPathMaterial();
    void closeEvent(QCloseEvent*event);

    void init(QString path);

public slots:
    void nodeSelected(NodeMaterial* node);
    void pickColor();


signals:
    void sendColor(glm::vec3 color);

private:
    Ui::MaterialEditorWindow *ui;
    QString m_pathMaterial;
};

#endif // MATERIALEDITORWINDOW_H
