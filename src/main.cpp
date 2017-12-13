#include "mainwindow.h"
#include "Editor/Utils/foldergestion.h"
#include <iostream>
#include <QApplication>
#include "Editor/Utils/projectinfo.h"
#include <QFile>
#include "Editor/Materials/materialeditorwindow.h"
#include "bullet/btBulletDynamicsCommon.h"

#pragma comment(lib, "shell32.lib")





int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    std::cout << "Skia Engine Run" << std::endl << std::endl;

    QFile file(":/DarkOrangeStyle.qss");
    if(file.open(QFile::ReadOnly)) {
       QString StyleSheet = QLatin1String(file.readAll());
       qApp->setStyleSheet(StyleSheet);
    }else{
        std::cout << "Cannot open Style Sheet" << std::endl;
    }
    if(!FolderGestion::isRootProjectsFolderExists()){
        FolderGestion::createRootProjectsFolder();
    }

    MainWindow w;
    w.show();


    return a.exec();
}
