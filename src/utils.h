#ifndef UTILS_H
#define UTILS_H
#include <vector>
#include <string>
#include <sstream>
#include "Editor/Materials/Nodes/nodematerial.h"
#include "src/Engine/Entities/actor.h"
#include "src/Editor/Utils/foldergestion.h"
#include "src/Editor/Utils/projectinfo.h"
#include <cstring>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>

class Utils
{
public:
    Utils();
    static float toRadians(float angle);
    static std::vector<std::string> split(std::string str, char delimiter);
    static std::string stringBetween(std::string str, char start,char stop);
    static QString removeExtension(QString fileName);
    static std::string getExtension(std::string fileName);
    static void createProcess(std::string cmd);
    static void shellExecute(std::string arg);
    static TypeVarType stringToTypeVar(QString str);
    static QString typeVarToString(TypeVarType type);
    static TypeVarType extToTypeVar(QString ext);
    static TypeVarType above(TypeVarType type);
    static TypeVarType addTypeVar(TypeVarType type1,TypeVarType type2);
    static std::wstring s2ws(const std::string& s);
private:

};

#endif // UTILS_H
