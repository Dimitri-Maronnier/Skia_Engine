#ifndef CONTENTBROWSER_H
#define CONTENTBROWSER_H
#include "ctreewidgetitem.h"
#include <QDir>
#include <QTreeWidgetItem>
#include <QFileInfoList>
#include <QTreeWidget>
#include <QDropEvent>
#include <QKeyEvent>
#include "src/Editor/Materials/materialeditorwindow.h"
#include "src/Editor/imageeditor.h"
#include "src/Editor/Objects3DStatic/object3dstaticeditor.h"

class ContentBrowser: public QTreeWidget
{
    Q_OBJECT
public:
    explicit ContentBrowser( QWidget *parent = 0);
    ~ContentBrowser();
    void reloadEditor();
public slots:
    void prepareMenuContentBrowser(const QPoint &);
    void newFolder();
    void removeItem();
    void importStaticMesh();
    void importTexture();
    void importHDRI();
    void newMat();
    void renameItem();
    void triggerRename();
private:
    int contain(QString str);
    void ContentBrowser::visitTree(QTreeWidgetItem *item, bool fill);
    void addChildren(CTreeWidgetItem *item, QString filePath);
    void dropEvent(QDropEvent* event);
    void keyPressEvent ( QKeyEvent * event );
    void keyReleaseEvent ( QKeyEvent * event );
    void mouseDoubleClickEvent(QMouseEvent * event);
    bool copy;
    struct ContentBrowserType{ QString name;bool expended;} ;
    MaterialEditorWindow *matEditor;
    ImageEditor *imageeditor;
    Object3DStaticEditor *objectEditor;
    std::vector<ContentBrowserType> holdTreeItems;
};

#endif // CONTENTBROWSER_H
