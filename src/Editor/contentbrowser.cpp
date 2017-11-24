#include "contentbrowser.h"
#include "src/Editor/Utils/projectinfo.h"
#include "src/Editor/Utils/foldergestion.h"
#include "src/Loaders/objloader.h"
#include "src/Loaders/imgloader.h"

#include <QFileDialog>
#include <QAction>
#include <QMenu>
#include <boost/filesystem.hpp>
#include "src/utils.h"
#include <iostream>
#include <Windows.h>
#include <QMimeData>
#include <QDrag>
#include <string>
#include "src/Engine/Materials/material.h"
#include "src/define.h"


ContentBrowser::ContentBrowser( QWidget *parent) : QTreeWidget(parent)
{
    connect(this,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(prepareMenuContentBrowser(const QPoint &)));
    connect(this,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(renameItem()));
    copy = false;
    reloadEditor();
    matEditor = NULL;
    imageeditor = NULL;
    objectEditor = NULL;
}

ContentBrowser::~ContentBrowser()
{

    SAFE_DELETE(matEditor);
    SAFE_DELETE(imageeditor);
    SAFE_DELETE(objectEditor);

}

void ContentBrowser::keyPressEvent ( QKeyEvent * event ){
    if(event->key() == Qt::Key_Alt){
        copy = true;
    }

}

void ContentBrowser::keyReleaseEvent ( QKeyEvent * event ){
    if(event->key() == Qt::Key_Alt){
        copy = false;
    }
}

void ContentBrowser::mouseDoubleClickEvent(QMouseEvent *event)
{

     CTreeWidgetItem *item = (CTreeWidgetItem*)this->itemAt( event->pos() );
     std::string path = FolderGestion::rootProjectsFolderPath;
     path += "\\";
     path += ProjectInfo::name;
     CTreeWidgetItem *upperParent = (CTreeWidgetItem *)this->itemAt( event->pos() );
     std::vector<std::string> reversePath;
     reversePath.push_back(upperParent->getOldName());
     while(upperParent->parent()){

        upperParent = (CTreeWidgetItem *)upperParent->parent();
        reversePath.push_back(upperParent->text(0).toStdString());
     }
     for (int i=reversePath.size()-1;i>=0;i--) {
        path += "\\";
        path += reversePath.at(i);


     }

     if(!item->isDirectory()){
         path += "." + item->getExt().toStdString();

     }

     if(item && !item->isDirectory()){
         if(item->getExt().compare("stex") == 0 || item->getExt().compare("shdrtex") == 0){
             if(imageeditor == NULL){
                imageeditor = new ImageEditor();
                imageeditor->init(QString(path.c_str()));
                imageeditor->show();
             }
             else{
                imageeditor->init(QString(path.c_str()));
                imageeditor->show();
             }
         }else if(item->getExt().compare("smat") == 0){
             if(matEditor == NULL){
                matEditor = new MaterialEditorWindow();
                matEditor->init(QString(path.c_str()));
                matEditor->show();
             }else{
                matEditor->init(QString(path.c_str()));
                matEditor->show();
             }

         }else if(item->getExt().compare("sobj") == 0){
             if(objectEditor == NULL){
                objectEditor = new Object3DStaticEditor();
                objectEditor->init(QString(path.c_str()));
                objectEditor->show();
             }else{
                objectEditor->init(QString(path.c_str()));
                objectEditor->show();
             }

         }
     }
}

void ContentBrowser::dropEvent(QDropEvent* event){
    CTreeWidgetItem *nd = (CTreeWidgetItem*)this->itemAt( event->pos() );
    CTreeWidgetItem *ndCopy = nd;
    if(nd && nd->isDirectory()){
        std::string path = FolderGestion::rootProjectsFolderPath;
        std::string npath = FolderGestion::rootProjectsFolderPath;
        path += "\\";
        path += ProjectInfo::name;
        npath += "\\";
        npath += ProjectInfo::name;
        CTreeWidgetItem *upperParent = (CTreeWidgetItem *)this->currentItem();
        std::vector<std::string> reversePath;
        std::string fileName = upperParent->text(0).toStdString();
        reversePath.push_back(upperParent->getOldName());
        while(upperParent->parent()){

           upperParent = (CTreeWidgetItem *)upperParent->parent();
           reversePath.push_back(upperParent->text(0).toStdString());
        }
        for (int i=reversePath.size()-1;i>=0;i--) {
           path += "\\";
           path += reversePath.at(i);


        }
        CTreeWidgetItem * currentItem = (CTreeWidgetItem*)this->currentItem();

        std::vector<std::string> reversePathTarget;
        reversePathTarget.push_back(nd->text(0).toStdString());
        while(nd->parent()){

           nd = (CTreeWidgetItem *)nd->parent();
           reversePathTarget.push_back(nd->text(0).toStdString());
        }
        for (int i=reversePathTarget.size()-1;i>=0;i--) {
           npath += "\\";
           npath += reversePathTarget.at(i);


        }
        if(!currentItem->isDirectory()){
            path += "." + currentItem->getExt().toStdString();
            npath += "\\";
            if(copy){
                npath += fileName;
                npath += "." + currentItem->getExt().toStdString();
            }
        }else if(copy){
          FolderGestion::createFolder(npath,fileName);
          npath += "\\";
          npath += fileName;
        }

        std::string cmd;

        if(copy)
            cmd = "copy " +  path + " " + npath;
        else
            cmd = "move " +  path + " " + npath;

        system(cmd.c_str());
        reloadEditor();
    }else if(!nd){
        std::string path = FolderGestion::rootProjectsFolderPath;
        std::string npath = FolderGestion::rootProjectsFolderPath;
        path += "\\";
        path += ProjectInfo::name;
        npath += "\\";
        npath += ProjectInfo::name;
        CTreeWidgetItem *upperParent = (CTreeWidgetItem *)this->currentItem();
        std::vector<std::string> reversePath;
        std::string fileName = upperParent->text(0).toStdString();
        reversePath.push_back(upperParent->getOldName());
        while(upperParent->parent()){

           upperParent = (CTreeWidgetItem *)upperParent->parent();
           reversePath.push_back(upperParent->text(0).toStdString());
        }
        for (int i=reversePath.size()-1;i>=0;i--) {
           path += "\\";
           path += reversePath.at(i);


        }
        CTreeWidgetItem * currentItem = (CTreeWidgetItem*)this->currentItem();

        if(!currentItem->isDirectory()){
            path += "." + currentItem->getExt().toStdString();
            npath += "\\";
            if(copy){
                npath += fileName;
                npath += "." + currentItem->getExt().toStdString();
            }
        }else if(copy){
          FolderGestion::createFolder(npath,fileName);
          npath += "\\";
          npath += fileName;
        }

        std::string cmd;

        if(copy)
            cmd = "copy " +  path + " " + npath;
        else
            cmd = "move " +  path + " " + npath;

        system(cmd.c_str());

        reloadEditor();
    }
}

void ContentBrowser::prepareMenuContentBrowser( const QPoint & pos )
{
    QTreeWidget *tree = this;

    QTreeWidgetItem *nd = tree->itemAt( pos );

    QAction *newAct = new QAction( tr("&New Folder"), this);
    newAct->setStatusTip(tr("new folder"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFolder()));
    QAction *remoteAct = new QAction( tr("&Remove"), this);
    remoteAct->setStatusTip(tr("remove item"));
    connect(remoteAct, SIGNAL(triggered()), this, SLOT(removeItem()));
    QAction *insertAct = new QAction( tr("&Import static mesh"), this);
    insertAct->setStatusTip(tr("import static mesh"));
    connect(insertAct, SIGNAL(triggered()), this, SLOT(importStaticMesh()));
    QAction *insertTex = new QAction( tr("&Import texture"), this);
    insertTex->setStatusTip(tr("import texture"));
    connect(insertTex, SIGNAL(triggered()), this, SLOT(importTexture()));
    QAction *insertHdri = new QAction( tr("&Import HDRI"), this);
    insertHdri->setStatusTip(tr("import HDRI"));
    connect(insertHdri, SIGNAL(triggered()), this, SLOT(importHDRI()));

    QAction *newMat = new QAction( tr("&New Material"), this);
    newMat->setStatusTip(tr("new Material"));
    connect(newMat, SIGNAL(triggered()), this, SLOT(newMat()));

    QAction *renameAct = new QAction( tr("&Rename"), this);
    renameAct->setStatusTip(tr("rename file/folder"));
    connect(renameAct, SIGNAL(triggered()), this, SLOT(triggerRename()));

    QMenu menu(this);
    QMenu *insertMenu = new QMenu();
    insertMenu->setTitle("Import");
    QMenu *newMenu = new QMenu();
    newMenu->setTitle("New");
    menu.addAction(newAct);
    menu.addAction(remoteAct);
    menu.addAction(renameAct);
    menu.addMenu(insertMenu);
    menu.addMenu(newMenu);
    insertMenu->addAction(insertAct);
    insertMenu->addAction(insertTex);
    insertMenu->addAction(insertHdri);

    newMenu->addAction(newMat);

    menu.exec( tree->mapToGlobal(pos) );
}

void ContentBrowser::importStaticMesh(){
    QString filter = "Wavefront format (*.obj);; Fbx(*.fbx);; Polygon format (*.ply)";

    QString fileName = QFileDialog::getOpenFileName(this, "Select a file...", QDir::homePath(), filter);

    if(fileName.at(0)!=0){
        std::string path = fileName.toStdString();
        std::vector<std::string> splitPath = Utils::split(path,'/');
        std::string nameFile = splitPath.at(splitPath.size()-1);
        nameFile = Utils::removeExtension(QString(nameFile.c_str())).toStdString();

        std::string pathProject = FolderGestion::rootProjectsFolderPath;
        pathProject += "\\";
        pathProject += ProjectInfo::name;
        CTreeWidgetItem *upperParent = (CTreeWidgetItem *)this->currentItem();
        if(upperParent){
            if(!upperParent->isDirectory() && upperParent->parent()){
                upperParent = (CTreeWidgetItem *)upperParent->parent();
                std::vector<std::string> reversePath;
                reversePath.push_back(this->currentItem()->text(0).toStdString());
                while(upperParent->parent()){

                   upperParent = (CTreeWidgetItem *)upperParent->parent();
                   reversePath.push_back(upperParent->text(0).toStdString());
                }
                for (int i=reversePath.size()-1;i>=0;i--) {
                   pathProject += "\\";
                   pathProject += reversePath.at(i);

                }
            }
        }

        pathProject += "\\";
        pathProject += nameFile;
        pathProject += ".sobj";


        if(!FolderGestion::isFileExists(pathProject)){

            CTreeWidgetItem* newItem = new CTreeWidgetItem();
            newItem->setIcon(0,*(new QIcon(":/Images/file.png")));
            newItem->setText(0,QString(nameFile.c_str()));
            newItem->setText(2,"sobj");
            newItem->setExt("sobj");
            newItem->setOldName(nameFile);
            newItem->setType(file);
            CTreeWidgetItem* copy = (CTreeWidgetItem *)this->currentItem();
            if(this->currentItem()){
                if(copy->parent())
                     copy->parent()->addChild(newItem);
                else
                    this->addTopLevelItem(newItem);
            }
            else{
                 this->addTopLevelItem(newItem);
            }

            //OBJLoader::loadObjModel(path,pathProject);
            OBJLoader::LoadModelFromFile(path,pathProject);

        }
    }
}

void ContentBrowser::importTexture()
{
    QString filter = "Image Files (*.png *.jpg *.bmp *.tga)";

    QString fileName = QFileDialog::getOpenFileName(this, "Select a file...", QDir::homePath(), filter);

    if(fileName.at(0)!=0){
        std::string path = fileName.toStdString();
        std::vector<std::string> splitPath = Utils::split(path,'/');
        std::string nameFile = splitPath.at(splitPath.size()-1);

        nameFile = Utils::removeExtension(QString(nameFile.c_str())).toStdString();

        std::string pathProject = FolderGestion::rootProjectsFolderPath;
        pathProject += "\\";
        pathProject += ProjectInfo::name;
        CTreeWidgetItem *upperParent = (CTreeWidgetItem *)this->currentItem();
        if(upperParent){
            if(!upperParent->isDirectory() && upperParent->parent()){
                upperParent = (CTreeWidgetItem *)upperParent->parent();
                std::vector<std::string> reversePath;
                reversePath.push_back(this->currentItem()->text(0).toStdString());
                while(upperParent->parent()){

                   upperParent = (CTreeWidgetItem *)upperParent->parent();
                   reversePath.push_back(upperParent->text(0).toStdString());
                }
                for (int i=reversePath.size()-1;i>=0;i--) {
                   pathProject += "\\";
                   pathProject += reversePath.at(i);

                }
            }
        }
        pathProject += "\\";
        pathProject += nameFile;
        pathProject += ".stex";


        if(!FolderGestion::isFileExists(pathProject)){

            CTreeWidgetItem* newItem = new CTreeWidgetItem();
            newItem->setIcon(0,*(new QIcon(":/Images/file.png")));
            newItem->setText(0,QString(nameFile.c_str()));
            newItem->setText(2,"stex");
            newItem->setExt("stex");
            newItem->setOldName(nameFile);
            newItem->setType(file);
            CTreeWidgetItem* copy = (CTreeWidgetItem *)this->currentItem();
            if(this->currentItem()){
                if(copy->parent())
                     copy->parent()->addChild(newItem);
                else
                    this->addTopLevelItem(newItem);
            }
            else{
                 this->addTopLevelItem(newItem);
            }

            ImgLoader::import(path,pathProject,false);

        }
    }
}

void ContentBrowser::importHDRI()
{
    QString filter = "High Dynamic Range Image (*.hdr)";

    QString fileName = QFileDialog::getOpenFileName(this, "Select a file...", QDir::homePath(), filter);

    if(fileName.at(0)!=0){
        std::string path = fileName.toStdString();
        std::vector<std::string> splitPath = Utils::split(path,'/');
        std::string nameFile = splitPath.at(splitPath.size()-1);

        nameFile = Utils::removeExtension(QString(nameFile.c_str())).toStdString();

        std::string pathProject = FolderGestion::rootProjectsFolderPath;
        pathProject += "\\";
        pathProject += ProjectInfo::name;
        CTreeWidgetItem *upperParent = (CTreeWidgetItem *)this->currentItem();
        if(upperParent){
            if(!upperParent->isDirectory() && upperParent->parent()){
                upperParent = (CTreeWidgetItem *)upperParent->parent();
                std::vector<std::string> reversePath;
                reversePath.push_back(this->currentItem()->text(0).toStdString());
                while(upperParent->parent()){

                   upperParent = (CTreeWidgetItem *)upperParent->parent();
                   reversePath.push_back(upperParent->text(0).toStdString());
                }
                for (int i=reversePath.size()-1;i>=0;i--) {
                   pathProject += "\\";
                   pathProject += reversePath.at(i);

                }
            }
        }

        pathProject += "\\";
        pathProject += nameFile;
        pathProject += ".shdrtex";


        if(!FolderGestion::isFileExists(pathProject)){

            CTreeWidgetItem* newItem = new CTreeWidgetItem();
            newItem->setIcon(0,*(new QIcon(":/Images/file.png")));
            newItem->setText(0,QString(nameFile.c_str()));
            newItem->setText(2,"shdrtex");
            newItem->setExt("shdrtex");
            newItem->setOldName(nameFile);
            newItem->setType(file);
            CTreeWidgetItem* copy = (CTreeWidgetItem *)this->currentItem();
            if(this->currentItem()){
                if(copy->parent())
                     copy->parent()->addChild(newItem);
                else
                    this->addTopLevelItem(newItem);
            }
            else{
                 this->addTopLevelItem(newItem);
            }

            ImgLoader::import(path,pathProject,true);

        }
    }
}

void ContentBrowser::newMat()
{


    std::string nameFile = "new";

    std::string pathProject = FolderGestion::rootProjectsFolderPath;
    pathProject += "\\";
    pathProject += ProjectInfo::name;
    CTreeWidgetItem *upperParent = (CTreeWidgetItem *)this->currentItem();
    if(upperParent){
        if(!upperParent->isDirectory() && upperParent->parent()){
            upperParent = (CTreeWidgetItem *)upperParent->parent();
            std::vector<std::string> reversePath;
            reversePath.push_back(this->currentItem()->text(0).toStdString());
            while(upperParent->parent()){

               upperParent = (CTreeWidgetItem *)upperParent->parent();
               reversePath.push_back(upperParent->text(0).toStdString());
            }
            for (int i=reversePath.size()-1;i>=0;i--) {
               pathProject += "\\";
               pathProject += reversePath.at(i);

            }
        }
    }
    pathProject += "\\";
    pathProject += nameFile;
    pathProject += ".smat";


    if(!FolderGestion::isFileExists(pathProject)){

        CTreeWidgetItem* newItem = new CTreeWidgetItem();
        newItem->setIcon(0,*(new QIcon(":/Images/file.png")));
        newItem->setText(0,QString(nameFile.c_str()));
        newItem->setText(2,"smat");
        newItem->setExt("smat");
        newItem->setOldName(nameFile);
        newItem->setType(file);
        CTreeWidgetItem* copy = (CTreeWidgetItem *)this->currentItem();
        if(this->currentItem()){
            if(copy->parent())
                 copy->parent()->addChild(newItem);
            else
                this->addTopLevelItem(newItem);
        }
        else{
             this->addTopLevelItem(newItem);
        }

        Material::newEmptyMat(QString(pathProject.c_str()));


    }
}

void ContentBrowser::triggerRename(){
    this->currentItem()->setFlags(this->currentItem()->flags() | Qt::ItemIsEditable);
    this->editItem(this->currentItem(),0);
    this->currentItem()->setFlags(this->currentItem()->flags() ^ Qt::ItemIsEditable);
}

void ContentBrowser::renameItem(){

    std::string path = FolderGestion::rootProjectsFolderPath;
    std::string npath = FolderGestion::rootProjectsFolderPath;
    path += "\\";
    path += ProjectInfo::name;
    npath += "\\";
    npath += ProjectInfo::name;
    CTreeWidgetItem *upperParent = (CTreeWidgetItem *)this->currentItem();
    std::vector<std::string> reversePath;
    reversePath.push_back(upperParent->getOldName());
    while(upperParent->parent()){

       upperParent = (CTreeWidgetItem *)upperParent->parent();
       reversePath.push_back(upperParent->text(0).toStdString());
    }
    for (int i=reversePath.size()-1;i>=0;i--) {
       path += "\\";
       path += reversePath.at(i);
       if(i>=1){
           npath += "\\";
           npath += reversePath.at(i);
       }

    }
    CTreeWidgetItem * currentItem = (CTreeWidgetItem*)this->currentItem();
    ;
    npath += "\\";
    npath += currentItem->text(0).toStdString();
    if(!currentItem->isDirectory()){
        path += "." + currentItem->getExt().toStdString();
        npath += "." + currentItem->getExt().toStdString();
    }
    boost::filesystem::path oldPath = path;
    boost::filesystem::path newPath = npath;
    boost::filesystem::rename(oldPath,newPath);
    upperParent->setOldName(upperParent->text(0).toStdString());

}

void ContentBrowser::newFolder(){



    std::string nameFile = "newFolder";

    std::string pathProject = FolderGestion::rootProjectsFolderPath;
    pathProject += "\\";
    pathProject += ProjectInfo::name;
    CTreeWidgetItem *upperParent = (CTreeWidgetItem *)this->currentItem();
    if(upperParent){
        if(!upperParent->isDirectory() && upperParent->parent()){
            upperParent = (CTreeWidgetItem *)upperParent->parent();
            std::vector<std::string> reversePath;
            reversePath.push_back(this->currentItem()->text(0).toStdString());
            while(upperParent->parent()){

               upperParent = (CTreeWidgetItem *)upperParent->parent();
               reversePath.push_back(upperParent->text(0).toStdString());
            }
            for (int i=reversePath.size()-1;i>=0;i--) {
               pathProject += "\\";
               pathProject += reversePath.at(i);

            }
        }
    }



    if(FolderGestion::createFolder(pathProject,"newFolder")){

        CTreeWidgetItem* newItem = new CTreeWidgetItem();
        newItem->setIcon(0,*(new QIcon(":/Images/folder.png")));
        newItem->setText(0,"newFolder");
        newItem->setOldName("newFolder");
        newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
        CTreeWidgetItem* copy = (CTreeWidgetItem *)this->currentItem();
        if(this->currentItem()){
            if(copy->parent())
                 copy->parent()->addChild(newItem);
            else
                this->addTopLevelItem(newItem);
        }
        else{
             this->addTopLevelItem(newItem);
        }
    }
}

void ContentBrowser::removeItem(){
    if(this->currentItem() && this->selectedItems().size()>0){
        std::string path = FolderGestion::rootProjectsFolderPath;
        path += "\\";
        path += ProjectInfo::name;
        CTreeWidgetItem *upperParent = (CTreeWidgetItem *)this->currentItem();
        CTreeWidgetItem *copy = upperParent;
        std::vector<std::string> reversePath;
        reversePath.push_back(this->currentItem()->text(0).toStdString());
        while(upperParent->parent()){

           upperParent = (CTreeWidgetItem *)upperParent->parent();
           reversePath.push_back(upperParent->text(0).toStdString());
        }
        for (int i=reversePath.size()-1;i>=0;i--) {
           path += "\\";
           path += reversePath.at(i);

        }

        if(!copy->isDirectory()){
            path += "." + copy->getExt().toStdString();

            /*std::string cmdS = "Del " + path;
            std::wstring wcmdS;
            std::copy(cmdS.c_str(), cmdS.c_str() + strlen(cmdS.c_str()), back_inserter(wcmdS));
            const WCHAR* cmd = wcmdS.c_str();
            std::cout << cmd << std::endl;
            CreateProcessW(NULL, (WCHAR*)cmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, NULL,NULL);*/
            boost::filesystem::path bpath = path;
            boost::filesystem::remove(bpath);
        }else{
            FolderGestion::removeFolder(path);
        }
        CTreeWidgetItem *toRemove = (CTreeWidgetItem *)this->currentItem();
        qDeleteAll(toRemove->takeChildren());
        //toRemove->parent()->removeChild(toRemove);
        delete toRemove;
    }
}

void ContentBrowser::visitTree(QTreeWidgetItem *item,bool fill){
    if(fill){
        ContentBrowserType temp;
        temp.name = item->text(0);
        temp.expended = item->isExpanded();
        holdTreeItems.push_back(temp);
    }else{
        int i = contain(item->text(0));
        if(i!=-1)
            item->setExpanded(holdTreeItems.at(i).expended);
    }
    for(int i=0;i<item->childCount(); ++i)
      visitTree(item->child(i),fill);
}

int ContentBrowser::contain(QString str){
    for(int i=0;i<holdTreeItems.size();i++)
        if(holdTreeItems.at(i).name == str)
            return i;
    return -1;
}

void ContentBrowser::reloadEditor(){
    for(int i=0;i<this->topLevelItemCount();++i)
      visitTree(this->topLevelItem(i),true);
    this->clear();

    char temp[MAX_PATH]="";
    strcat(temp,FolderGestion::rootProjectsFolderPath);
    strcat(temp,"\\");
    QDir* rootDir = new QDir(strcat(temp,ProjectInfo::name.c_str()));

    QFileInfoList filesList = rootDir->entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries);


    /*CTreeWidgetItem* headerItem = new CTreeWidgetItem();
    headerItem->setText(0,QString("File Name"));
    headerItem->setText(1,QString("Size (Bytes)"));
    headerItem->setText(2,QString("Type"));
    this->setHeaderItem(headerItem);*/


    foreach(QFileInfo fileInfo, filesList)
    {
      CTreeWidgetItem* item = new CTreeWidgetItem();
      item->setText(0,fileInfo.baseName());
      item->setOldName(fileInfo.baseName().toStdString());

      if(fileInfo.isFile())
      {
        item->setText(1,QString::number(fileInfo.size()));
        item->setIcon(0,*(new QIcon(":/Images/file.png")));
        item->setText(2,fileInfo.suffix());
        item->setExt(fileInfo.suffix());
        item->setType(file);
      }

      if(fileInfo.isDir())
      {
        item->setIcon(0,*(new QIcon(":/Images/folder.png")));
        item->setType(directory);

        //item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
        addChildren(item,fileInfo.filePath());
      }


      this->addTopLevelItem(item);
    }
    for(int i=0;i<this->topLevelItemCount();++i)
      visitTree(this->topLevelItem(i),false);
}

void ContentBrowser::addChildren(CTreeWidgetItem* item,QString filePath)
{
    QDir* rootDir = new QDir(filePath);
    QFileInfoList filesList = rootDir->entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries);

    foreach(QFileInfo fileInfo, filesList)
    {
        CTreeWidgetItem* child = new CTreeWidgetItem();
        child->setText(0,fileInfo.baseName());
        child->setOldName(fileInfo.baseName().toStdString());

        if(fileInfo.isFile())
        {
          child->setText(1,QString::number(fileInfo.size()));
          child->setIcon(0,*(new QIcon(":/Images/file.png")));
          child->setText(2,fileInfo.suffix());
          child->setExt(fileInfo.suffix());
          child->setType(file);
        }

        if(fileInfo.isDir())
        {
          child->setIcon(0,*(new QIcon(":/Images/folder.png")));
          child->setType(directory);

          //child->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
          addChildren(child,fileInfo.filePath());
        }

        item->addChild(child);

    }
}
