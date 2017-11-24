#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Editor/newprojectdialog.h"

#include "Editor/Utils/projectinfo.h"
#include "Editor/ctreewidgetitem.h"
#include "src/Assets/assetscollections.h"

QGLContext *MainWindow::contxt;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    menuBar()->actions().first()->menu()->actions().first()->setStatusTip(tr("Creating project"));
    connect( menuBar()->actions().first()->menu()->actions().first(), SIGNAL(triggered()), this, SLOT(newProject()));
    ui->contentBrowserTree->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->ProjectName->setText(QString(ProjectInfo::name.c_str()));

    QGLFormat format;
    contxt=new QGLContext(format);

    CTreeWidgetItem* headerItem = new CTreeWidgetItem();
    headerItem->setText(0,QString("File Name"));
    headerItem->setText(1,QString("Size (Bytes)"));
    headerItem->setText(2,QString("Type"));
    ui->contentBrowserTree->header()->setVisible(true);
    ui->contentBrowserTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->contentBrowserTree->setHeaderItem(headerItem);
    ui->contentBrowserTree->setIconSize(QSize(32,32));
    connect(ui->GL->getScene(),SIGNAL(oneEntityHaveBeenSelected(Object3DStatic*)),ui->DetailsToolBox,SLOT(initDetailArena(Object3DStatic*)));

}



MainWindow::~MainWindow()
{
    AssetsCollections::cleanUp();
    delete ui;
}

void MainWindow::newProject(){
    NewProjectDialog newProjectDialog;
    if(newProjectDialog.exec() == QDialog::Accepted){
        ui->contentBrowserTree->reloadEditor();
    }
}
