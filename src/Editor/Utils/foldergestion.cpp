#include "foldergestion.h"
#include <iostream>
#include <QDir>
#include <QFileInfoList>
#include "src/Editor/Utils/projectinfo.h"
#include "src/utils.h"

char FolderGestion::rootProjectsFolderPath[MAX_PATH] = "";
char FolderGestion::my_documentsPath[MAX_PATH] = "";
WCHAR FolderGestion::rootProjectsFolderPathAddr[MAX_PATH];
WCHAR FolderGestion::my_documentsAddr[MAX_PATH];
std::string FolderGestion::currentWorkingDir;
FolderGestion::FolderGestion()
{

}

bool FolderGestion::isRootProjectsFolderExists(){

    HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, my_documentsAddr);

    for(int i=0;i<MAX_PATH;i++){
        my_documentsPath[i] = my_documentsAddr[i];
    }

    struct stat sb;
    char* ProjectFolder = (char*)"\\SkiaProjects";
    for(int i = 0;i<15;i++){
        rootProjectsFolderPathAddr[i] = ProjectFolder[i];
    }

    strcat(rootProjectsFolderPath,my_documentsPath);
    strcat(rootProjectsFolderPath,ProjectFolder);
    boost::filesystem::path bPath(rootProjectsFolderPath);
    return boost::filesystem::exists(bPath);

}

int FolderGestion::createRootProjectsFolder(){

    std::cout << "Creating Folder Projects at " << my_documentsPath << std::endl;
    if(CreateDirectory(wcsncat(my_documentsAddr,rootProjectsFolderPathAddr,MAX_PATH),NULL)){
        std::cout << "Folder create" << std::endl;
    }
    else{
        std::cout << "Cannot create folder" << std::endl;
    }
    boost::filesystem::path bPath(rootProjectsFolderPath);
    bPath += "/";
    bPath += "default";
    boost::filesystem::create_directory(bPath);
    return 1;
}

int FolderGestion::createProjectFolder(std::string name){
    struct stat sb;
    char copyRootProjectsFolderPath[MAX_PATH] = "";
    strcat(copyRootProjectsFolderPath,rootProjectsFolderPath);
    name.insert(0,"\\",1);
    boost::filesystem::path bPath(strcat(copyRootProjectsFolderPath,name.c_str()));

    if(! boost::filesystem::exists(bPath)){

        WCHAR ProjectFolderAddr[MAX_NAME];
        int i;
        for(i=0; i< name.size();i++){
            if(i<MAX_NAME){
                ProjectFolderAddr[i] = name.at(i);
            }
        }
        ProjectFolderAddr[i] = 0;
        WCHAR copyRootProjectsFolderPathAddr[MAX_PATH];
        wcscpy ( copyRootProjectsFolderPathAddr, L"" );
        wcsncat(copyRootProjectsFolderPathAddr,my_documentsAddr,MAX_PATH);
        wcsncat(copyRootProjectsFolderPathAddr,rootProjectsFolderPathAddr,MAX_PATH);


        if(CreateDirectory(wcsncat(copyRootProjectsFolderPathAddr,ProjectFolderAddr,MAX_PATH),NULL)){
            std::cout << "Folder create" << std::endl;
            FolderGestion::currentWorkingDir = rootProjectsFolderPath;
            FolderGestion::currentWorkingDir += "\\";
            FolderGestion::currentWorkingDir += name;
            boost::filesystem::path bPath2 = bPath;
            bPath += "/modules";
            bPath2 += "/contents";
            boost::filesystem::create_directory(bPath);
            boost::filesystem::create_directory(bPath2);
            return 1;
        }
        else{
            std::cout << "Cannot create folder" << std::endl;
            return 0;
        }


    }
    return -1;
}

int FolderGestion::createFolder(std::string path,std::string name){
    struct stat sb;
    name.insert(0,"\\",1);
    boost::filesystem::path bPath(strcat((char*)path.c_str(),name.c_str()));
    if(!boost::filesystem::exists(bPath)){

        WCHAR FolderAddr[MAX_NAME];
        int i;
        for(i=0; i< name.size();i++){
            if(i<MAX_NAME){
                FolderAddr[i] = name.at(i);
            }
        }
        FolderAddr[i] = 0;
        WCHAR PathAddr[MAX_PATH];
        for(i=0; i< path.size();i++){
            if(i<MAX_PATH){
                PathAddr[i] = path.at(i);
            }
        }
        PathAddr[i] = 0;

        if(CreateDirectory(wcsncat(PathAddr,FolderAddr,MAX_PATH),NULL)){
            std::cout << "Folder create" << std::endl;
            return 1;
        }
        else{
            std::cout << "Cannot create folder" << std::endl;
            return 0;
        }


    }
    return -1;
}

int FolderGestion::removeFolder(std::string path){
    boost::filesystem::path bpath = path;
    boost::filesystem::remove_all(bpath);
    return 1;
}

bool FolderGestion::isFileExists(std::string path){
    boost::filesystem::path bPath(path);
    return boost::filesystem::exists(bPath);
}

void FolderGestion::recursive_copy(const boost::filesystem::path &src, const boost::filesystem::path &dst)
{
  if (!boost::filesystem::exists(dst)){
    return;
  }

  if (boost::filesystem::is_directory(src)) {

    for (boost::filesystem::directory_entry& item : boost::filesystem::directory_iterator(src)) {
      recursive_copy(item.path(), dst/item.path().filename());
    }
  }
  else if (boost::filesystem::is_regular_file(src)) {
    boost::filesystem::copy(src, dst);
  }
}

QString FolderGestion::addProjectPath(const QString str)
{
    QString path = QString(currentWorkingDir.c_str());
    path += str;
    return path;
}

QString FolderGestion::subFolderCheckout(const QString filePath,const std::string name, const QString path)
{
    QDir* rootDir = new QDir(filePath);
    QFileInfoList filesList = rootDir->entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries);

    foreach(QFileInfo fileInfo, filesList)
    {

        if(fileInfo.isFile() && !(fileInfo.baseName() + "." + fileInfo.suffix()).toStdString().compare(name))
        {

            if(!fileInfo.path().compare(path)){
                return path + "\\" + QString(name.c_str()); //The reference still right
            }else{//Need to rebuild reference
                return fileInfo.path() + "\\" + QString(name.c_str());
            }

        }

        if(fileInfo.isDir())
        {
            return subFolderCheckout(fileInfo.filePath(),name,path);
        }


    }
    return "";
}

QString FolderGestion::checkoutReferences(const QString path)
{
    std::string strRef = path.toStdString();
    std::replace( strRef.begin(), strRef.end(), '/', '\\');
    const std::vector<std::string> split = Utils::split(strRef,'\\');

    const std::string name = (split.size()>0)?split.at(split.size()-1):path.toStdString();
    char temp[MAX_PATH]="";

    QDir* rootDir = new QDir(currentWorkingDir.c_str());

    QFileInfoList filesList = rootDir->entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries);


    foreach(QFileInfo fileInfo, filesList)
    {

        if(fileInfo.isFile() && !(fileInfo.baseName() + "." + fileInfo.suffix()).toStdString().compare(name))//The reference still right
        {
            return FolderGestion::addProjectPath( "\\" + QString(name.c_str()));
        }

        if(fileInfo.isDir())
        {
            QString found = subFolderCheckout(fileInfo.filePath(),name,path);
            if(found.size()>0){
                if(found.at(0) == "/" || found.at(0) == "\\"){
                    return FolderGestion::addProjectPath(found);
                }
                return found;
            }
        }

    }
    return "";
}


QString FolderGestion::removeProjectPath(const QString str)
{
    std::string varUtf8 = str.toUtf8().constData();
    unsigned int position = 0;
    while(position<MAX_PATH && currentWorkingDir[position] == varUtf8.at(position))
        position++;
    std::string var = varUtf8.substr(position,varUtf8.size()-1);
    return QString(var.c_str());
}

