#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Editor/newprojectdialog.h"

#include "Editor/Utils/projectinfo.h"
#include "Editor/ctreewidgetitem.h"
#include "src/Assets/assetscollections.h"
#include "src/Editor/Utils/foldergestion.h"
#include "src/Editor/Utils/compressor.h"
#include "src/Game/game.h"
#include <Xinput.h>

#include <QFileDialog>

QGLContext *MainWindow::contxt;
bool MainWindow::ContextBlock;
GLWidget *MainWindow::mainGl;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    menuBar()->actions().first()->menu()->actions().first()->setStatusTip(tr("Creating project"));
    menuBar()->actions().first()->menu()->actions().at(1)->setStatusTip("Opening project");
    connect( menuBar()->actions().first()->menu()->actions().first(), SIGNAL(triggered()), this, SLOT(newProject()));
    connect(menuBar()->actions().first()->menu()->actions().at(1), SIGNAL(triggered()), this, SLOT(openProject()));
    ui->contentBrowserTree->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->ProjectName->setText(QString(ProjectInfo::name.c_str()));

    QGLFormat format;
    contxt=new QGLContext(format);
    mainGl = ui->GL;
    CTreeWidgetItem* headerItem = new CTreeWidgetItem();
    headerItem->setText(0,QString("File Name"));
    headerItem->setText(1,QString("Size (Bytes)"));
    headerItem->setText(2,QString("Type"));
    ui->contentBrowserTree->header()->setVisible(true);
    ui->contentBrowserTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->contentBrowserTree->setHeaderItem(headerItem);
    ui->contentBrowserTree->setIconSize(QSize(32,32));
    connect(ui->GL->getScene(),SIGNAL(oneEntityHaveBeenSelected(Object3DStatic*)),ui->DetailsToolBox,SLOT(initDetailArena(Object3DStatic*)));
    connect(ui->actionSave_Map,SIGNAL(triggered(bool)),this,SLOT(saveMap()));
    connect(ui->contentBrowserTree,SIGNAL(mapSelected(QString)), ui->GL->getScene(),SLOT(setScene(QString)));
    connect(ui->PlayButton,SIGNAL(clicked(bool)),ui->GL,SLOT(gameRun()));
    connect(ui->compileButton,SIGNAL(clicked(bool)),this,SLOT(compileGame()));
    this->openProject();
    ProjectInfo::loadModules();

}



MainWindow::~MainWindow()
{
    AssetsCollections::cleanUp();
    ProjectInfo::cleanUp();
    delete ui;
}

SceneTree *MainWindow::getSceneTree()
{
    return ui->treeWidget;
}

void MainWindow::newProject(){
    NewProjectDialog newProjectDialog;
    if(newProjectDialog.exec() == QDialog::Accepted){
        ui->contentBrowserTree->reloadEditor();
        ui->ProjectName->setText(QString(ProjectInfo::name.c_str()));
    }
}

void MainWindow::openProject()
{
    QString fileName =
                QFileDialog::getExistingDirectory(nullptr,
                                             QObject::tr("Open Project"),
                                             QString(FolderGestion::rootProjectsFolderPath));

    if(fileName.size()>0){
        std::replace( fileName.begin(), fileName.end(), '/', '\\');
        std::vector<std::string> split = Utils::split(fileName.toStdString(),'\\');
        ProjectInfo::name = split.at(split.size()-1);
        FolderGestion::currentWorkingDir = fileName.toStdString();
        ui->contentBrowserTree->reloadEditor();
        ui->ProjectName->setText(QString(split.at(split.size()-1).c_str()));
    }else{
        FolderGestion::currentWorkingDir = FolderGestion::rootProjectsFolderPath;
        FolderGestion::currentWorkingDir += "\\";
        FolderGestion::currentWorkingDir += ProjectInfo::name;
        ui->contentBrowserTree->reloadEditor();
        ui->ProjectName->setText(QString(ProjectInfo::name.c_str()));
    }

}

void MainWindow::saveMap()
{
    Compressor::compressMap(ProjectInfo::currentMap,*ui->GL->getScene());
}

void MainWindow::gameRun()
{
    GLWidget::GameRun = true;

}

void MainWindow::compileGame()
{
    ProjectInfo::compileModules();
}
