#ifndef FOLDERGESTION_H
#define FOLDERGESTION_H
#include <windows.h>
#include <shlobj.h>
#include <sys/stat.h>
#include <wchar.h>
#include <string>
#include <boost/filesystem.hpp>
#include <QString>
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
    static std::string currentWorkingDir;
    static char my_documentsPath[MAX_PATH];
    static WCHAR my_documentsAddr[MAX_PATH];
    static void recursive_copy(const boost::filesystem::path &src, const boost::filesystem::path &dst);

    /**
     *\brief Delete project path from incomming argument
     *
     */
    static QString removeProjectPath(const QString str);

    /**
     * @brief Add Project Path to str
     * @param str
     * @return
     */
    static QString addProjectPath(const QString str);

    /**
     * @brief checkoutReferences and rebuild if needed
     * @param str
     */
    static QString checkoutReferences(const QString path);

private :
    static WCHAR rootProjectsFolderPathAddr[MAX_PATH];
    static QString FolderGestion::subFolderCheckout(const QString filePath, const std::string name, const QString path);
};


#endif // FOLDERGESTION_H
