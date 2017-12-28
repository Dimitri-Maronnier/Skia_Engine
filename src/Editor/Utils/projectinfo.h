#ifndef PROJECTINFO_H
#define PROJECTINFO_H
#include <string>
#include <QString>
#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>
#include <exception>
#include <windows.h>
#include "src/Engine/Entities/actor.h"
#include "src/utils.h"
#include "src/Editor/Utils/foldergestion.h"
#include <QLibrary>

class ProjectInfo
{
public:
    ProjectInfo();
    static void loadModules();
    static void compileModules();
    static void freeModules();
    static void cleanUp();
    static std::string name;
    static std::string description;
    static QString currentMap;
    static std::vector<HINSTANCE> modules;
    static std::vector<std::unique_ptr<Actor>> actors;
private:
    static void recursiveFindModule(const QString &filePath);
    static std::vector<QLibrary*> libs;
};

#endif // PROJECTINFO_H
