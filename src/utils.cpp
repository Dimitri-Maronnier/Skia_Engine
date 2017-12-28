#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>
#include <string>
#include <iostream>
#include "utils.h"

Utils::Utils()
{

}

float Utils::toRadians(float angle){
    return angle * M_PI / 180.0 ;
}

std::vector<std::string> Utils::split(std::string str, char delimiter) {
  std::vector<std::string> internal;
  std::stringstream ss(str); // Turn the string into a stream.
  std::string tok;

  while(std::getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }

  return internal;
}

std::string Utils::stringBetween(std::string str, char start,char stop){
    unsigned first = str.find(start);
    str.at(first) = 0;
    unsigned last = str.find(stop);

    std::string strNew = str.substr (first+1,last-first-1);
    return strNew;
}

QString Utils::removeExtension(QString fileName)
{
    for(int i=fileName.size();i>0;i--){
        if(fileName.at(i) == '.'){
            fileName.remove(i,fileName.size()-i);
            break;
        }
    }
    return fileName;
}

std::string Utils::getExtension(std::string fileName)
{
    size_t pos = fileName.find('.');
    std::string ext = fileName.substr(pos);
    return ext;
}

void Utils::createProcess(std::string cmd){

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );
    WCHAR wcmd[MAX_PATH];
    int i;
    for(i=0;i<cmd.size();i++)
        wcmd[i] = cmd.at(i);
    wcmd[i] = 0;

    if( !CreateProcess( NULL,   // No module name (use command line)
        wcmd,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        CREATE_NO_WINDOW,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory
        &si,            // Pointer to STARTUPINFO structure
        &pi )           // Pointer to PROCESS_INFORMATION structure
    )
    {
        std::cerr << "CreateProcess failed " << GetLastError() << std::endl;
        return;
    }

    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles.
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
}

void Utils::shellExecute(std::string arg)
{


    WCHAR wcmd[MAX_PATH];
    int i;
    for(i=0;i<arg.size();i++)
        wcmd[i] = arg.at(i);
    wcmd[i] = 0;
    ShellExecute( NULL,  L"open",
        L"command",
        wcmd,
        L"c:\\",
        SW_SHOWNORMAL
    );

}

TypeVarType Utils::stringToTypeVar(QString str)
{
    if(str.compare("mat4") == 0){
        return mat4;
    }else if(str.compare("mat3") == 0){
        return mat3;
    }else if(str.compare("mat2") == 0){
        return mat2;
    }else if(str.compare("vec4") == 0){
        return vec4;
    }else if(str.compare("vec3") == 0){
        return vec3;
    }else if(str.compare("vec2") == 0){
        return vec2;
    }else if(str.compare("float") == 0){
        return Float;
    }else if(str.compare("bool") == 0){
        return Bool;
    }else{
        return Default;
    }
}

QString Utils::typeVarToString(TypeVarType type)
{
    switch(type){
        case mat4:
            return "mat4";
        case mat3:
            return "mat3";
        case mat2:
            return "mat2";
        case vec4:
            return "vec4";
        case vec3:
            return "vec3";
        case vec2:
            return "vec2";
        case Float:
            return "float";
        case Bool:
            return "bool";
        default:
            return "";
    }
}

TypeVarType Utils::extToTypeVar(QString ext)
{
    std::vector<std::string> split = Utils::split(ext.toStdString(),'.');

    if(split.at(1).size()==1){
        return Float;
    }else if(split.at(1).size()==2){
        return vec2;
    }else if(split.at(1).size()==3){
        return vec3;
    }
}

TypeVarType Utils::above(TypeVarType type)
{
    switch(type){
        case vec4:
            return vec4;
        case vec3:
            return vec4;
        case vec2:
            return vec3;
        case Float:
            return vec2;
        case Bool:
            return Bool;
        default:
            return Float;
    }
}

TypeVarType Utils::addTypeVar(TypeVarType type1, TypeVarType type2)
{
    switch(type1){
        case vec3:
            switch(type2){
                case Float:
                    return vec4;
                default:
                    return Float;
            }
        case vec2:
            switch(type2){

                case vec2:
                    return vec4;
                case Float:
                    return vec3;
                default:
                    return Float;
            }
        case Float:
            switch(type2){

                case vec3:
                    return vec4;
                case vec2:
                    return vec3;
                case Float:
                    return vec2;
                default:
                    return Float;
            }
        default:
            return Float;
    }
}

std::wstring Utils::s2ws(const std::string& s)
{
 int len;
 int slength = (int)s.length() + 1;
 len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
 wchar_t* buf = new wchar_t[len];
 MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
 std::wstring r(buf);
 delete[] buf;
 return r;
}
