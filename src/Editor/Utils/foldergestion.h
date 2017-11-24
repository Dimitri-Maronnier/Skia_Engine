#ifndef FOLDERGESTION_H
#define FOLDERGESTION_H
#include <windows.h>
#include <shlobj.h>
#include <sys/stat.h>
#include <wchar.h>
#include <string>
#include <boost/filesystem.hpp>
#define MAX_NAME 256

class FolderGestion
{
public:
    FolderGestion();
    static bool isRootProjectsFolderExists();
    static int createRootProjectsFolder();
    static int createProjectFolder(std::string name);
    static int createFolder(std::string path,std::string name);
    static int removeFolder(std::string path);
    static bool isFileExists(std::string path);
    static char rootProjectsFolderPath[MAX_PATH];
    static char my_documentsPath[MAX_PATH];
    static WCHAR my_documentsAddr[MAX_PATH];
    static void recursive_copy(const boost::filesystem::path &src, const boost::filesystem::path &dst);
private :
    static WCHAR rootProjectsFolderPathAddr[MAX_PATH];
};


#endif // FOLDERGESTION_H
