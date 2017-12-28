#include "projectinfo.h"
#include <QDir>


std::string ProjectInfo::name = "default";
std::string ProjectInfo::description = "";
QString ProjectInfo::currentMap = "default.smap";
std::vector<HINSTANCE> ProjectInfo::modules;
std::vector<std::unique_ptr<Actor>> ProjectInfo::actors;
std::vector<QLibrary*> ProjectInfo::libs;

ProjectInfo::ProjectInfo()
{

}

void ProjectInfo::loadModules()
{
    char temp[MAX_PATH]="";
    strcat(temp,FolderGestion::rootProjectsFolderPath);
    strcat(temp,"\\");
    QDir* rootDir = new QDir(FolderGestion::currentWorkingDir.c_str());

    QFileInfoList filesList = rootDir->entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries);

    foreach(QFileInfo fileInfo, filesList)
    {
        if(!fileInfo.suffix().compare("dll")){
            QLibrary* lib = new QLibrary( fileInfo.filePath() );
            libs.push_back(lib);

            typedef Actor* (*create_DLL_lib_fun)();

            if(!lib->load()){
                qDebug() << "couldn't load library: " + lib->errorString();
            }else{
                create_DLL_lib_fun con = (create_DLL_lib_fun) lib->resolve("create_DLL_lib");
                if (con){
                    Actor *actorInstance = con();
                    Actor::Actors.push_back(actorInstance);
                    //actorInstance->setUp(nullptr);
                }


            }
        }
        if(fileInfo.isDir()){
            recursiveFindModule(fileInfo.filePath());
        }
    }
}

void ProjectInfo::compileModules()
{
    foreach(Actor* actor,Actor::Actors){
        SAFE_DELETE(actor);
    }
    Actor::Actors.clear();
    foreach(QLibrary* lib,libs){
        lib->unload();

        //Execute commande qmake & compiler
        //qmake.exe test.pro -spec win32-msvc2015
        //jom.exe in C:\Users\C17\Documents\SkiaProjects\test\modules
        //jom.exe clean in C:\Users\C17\Documents\SkiaProjects\test\modules
        //ShellExecuteW(NULL, NULL, L"qmake.exe test.pro -spec win32-msvc2015", NULL, NULL, SW_SHOWNORMAL);
        /*system("C:\\Qt\\Qt5.8.0\\5.8\\msvc2015_64\\bin\\qmake.exe C:\\Users\\C17\\Documents\\SkiaProjects\\test\\modules\\test.pro -spec win32-msvc2015");
        //ShellExecuteW(NULL, NULL, L"jom.exe in C:\\Users\\C17\\Documents\\SkiaProjects\\test\\modules", NULL, NULL, SW_SHOWNORMAL);
        system("C:\\Qt\\Qt5.8.0\\Tools\\QtCreator\\bin\\jom.exe in C:\\Users\\C17\\Documents\\SkiaProjects\\test\\modules");
        //ShellExecuteW(NULL, NULL, L"jom.exe clean in C:\\Users\\C17\\Documents\\SkiaProjects\\test\\modules", NULL, NULL, SW_SHOWNORMAL);
        system("C:\\Qt\\Qt5.8.0\\Tools\\QtCreator\\bin\\jom.exe clean in C:\\Users\\C17\\Documents\\SkiaProjects\\test\\modules");*/

        typedef Actor* (*create_DLL_lib_fun)();

        if(!lib->load()){
            qDebug() << "couldn't load library: " + lib->errorString();
        }else{
            create_DLL_lib_fun con = (create_DLL_lib_fun) lib->resolve("create_DLL_lib");
            if (con){
                Actor *actorInstance = con();
                Actor::Actors.push_back(actorInstance);
                //actorInstance->setUp(nullptr);
            }


        }
    }
}

void ProjectInfo::freeModules()
{
    for (HINSTANCE hInst : modules)
        FreeLibrary(hInst);
}

void ProjectInfo::cleanUp()
{
    foreach(QLibrary* lib,libs){
        lib->unload();
        SAFE_DELETE(lib);
    }
}

void ProjectInfo::recursiveFindModule(const QString &filePath)
{
    QDir* rootDir = new QDir(filePath);
    QFileInfoList filesList = rootDir->entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries);
    foreach(QFileInfo fileInfo, filesList)
    {
        if(!fileInfo.suffix().compare("dll")){
            QLibrary* lib = new QLibrary( fileInfo.filePath() );
            libs.push_back(lib);

            typedef Actor* (*create_DLL_lib_fun)();

            if(!lib->load()){
                qDebug() << "couldn't load library: " + lib->errorString();
            }else{
                create_DLL_lib_fun con = (create_DLL_lib_fun) lib->resolve("create_DLL_lib");
                if (con){
                    Actor *actorInstance = con();
                    Actor::Actors.push_back(actorInstance);
                    //actorInstance->setUp(nullptr);
                }
            }

        }
        if(fileInfo.isDir()){
            recursiveFindModule(fileInfo.filePath());
        }
    }
}

