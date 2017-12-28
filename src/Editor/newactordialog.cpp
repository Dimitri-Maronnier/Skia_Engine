#include "newactordialog.h"
#include "ui_newactordialog.h"
#include <iostream>
#include "Utils/foldergestion.h"
#include "Utils/projectinfo.h"

QString NewActorDialog::DefaultHeader =
        "#include <QtCore/QtGlobal>\n"
        "#include \"src/Engine/Entities/actor.h\"\n"

        "#if defined //#UPPERCASE\n"
        " #define //#UPPERCASE_COMMON_DLLSPEC Q_DECL_EXPORT\n"
        "#else\n"
        " #define //#UPPERCASE_COMMON_DLLSPEC Q_DECL_IMPORT\n"
        "#endif\n"

        "class //#UPPERCASE_COMMON_DLLSPEC //#className:public Actor\n"
        "{\n"
        " public:\n"
        "  //#className();\n"
        "  void setUp()override;\n"
        "  void update(double deltaTime)override;\n"
        "};\n"

        "extern \"C\" //#UPPERCASE_COMMON_DLLSPEC Actor *create_Dll_lib()\n"
        "{\n"
        "    return new //#className();\n"
        "}";

QString NewActorDialog::DefaultSource =
        "#include \"//#LOWER.h\"\n"
        "#include <iostream>\n"

        "//#className:://#className() {\n"
        "}\n"

        "void //#className::setUp()\n"
        "{\n"
        "}\n"

        "void //#className::update(double deltaTime)\n"
        "{\n"
        "}";

QString NewActorDialog::DefaultPro =
        "TEMPLATE = lib\n"
        "INCLUDEPATH += //#PATH\n"
        "CONFIG += c++11\n"
        "TARGET = //#LOWER\n"
        "# Input\n"
        "SOURCES += //#LOWER.cpp\n"
        "HEADERS += //#LOWER.h\n"
        "DEFINES += //#UPPER";


NewActorDialog::NewActorDialog( QWidget * parent) : QDialog(parent),
    ui(new Ui::NewActorDialog)
{
    ui->setupUi(this);
}

void NewActorDialog::accept(){

    std::string path = FolderGestion::rootProjectsFolderPath;
    path += "/";
    path = FolderGestion::currentWorkingDir;
    path += "\\modules";
    std::cout << path << std::endl;
    if(FolderGestion::createFolder(path,ui->name->text().toStdString())){
        QString upper = ui->name->text().toUpper();
        QString lower = ui->name->text().toLower();
        QString header = DefaultHeader;
        QString source = DefaultSource;
        QString pro = DefaultPro;

        size_t posStr = header.toStdString().find("//#className");
        while(posStr != std::string::npos){

            header.replace(posStr,QString("//#className").length(),ui->name->text());
            posStr = header.toStdString().find("//#className");
        }

        posStr = header.toStdString().find("//#UPPERCASE");
        while(posStr != std::string::npos){

            header.replace(posStr,QString("//#UPPERCASE").length(),upper);
            posStr = header.toStdString().find("//#UPPERCASE");
        }



        posStr = source.toStdString().find("//#className");
        while(posStr != std::string::npos){

            source.replace(posStr,QString("//#className").length(),ui->name->text());
            posStr = source.toStdString().find("//#className");
        }

        posStr = source.toStdString().find("//#LOWER");
        while(posStr != std::string::npos){

            source.replace(posStr,QString("//#LOWER").length(),lower);
            posStr = source.toStdString().find("//#LOWER");
        }

        posStr = pro.toStdString().find("//#UPPER");
        while(posStr != std::string::npos){

            pro.replace(posStr,QString("//#UPPER").length(),upper);
            posStr = pro.toStdString().find("//#UPPER");
        }

        posStr = pro.toStdString().find("//#LOWER");
        while(posStr != std::string::npos){

            pro.replace(posStr,QString("//#LOWER").length(),lower);
            posStr = pro.toStdString().find("//#LOWER");
        }

        posStr = pro.toStdString().find("//#PATH");
        while(posStr != std::string::npos){

            pro.replace(posStr,QString("//#PATH").length(),qApp->applicationDirPath().toUtf8());
            posStr = pro.toStdString().find("//#PATH");
        }


        QFile* sourceFile = new QFile(QString(path.c_str()) + "/" + ui->name->text() + "/" + lower + ".cpp");
        sourceFile->open(QIODevice::WriteOnly);
        if(sourceFile->isOpen())
        {
          sourceFile->write(source.toUtf8());
        }
        sourceFile->close();
        delete sourceFile;

        QFile* headerFile = new QFile(QString(path.c_str()) + "/" + ui->name->text() + "/" + lower + ".h");
        headerFile->open(QIODevice::WriteOnly);
        if(headerFile->isOpen())
        {
          headerFile->write(header.toUtf8());
        }
        headerFile->close();
        delete headerFile;

        QFile* proFile = new QFile(QString(path.c_str()) + "/" + ui->name->text() + "/" + lower + ".pro");
        proFile->open(QIODevice::WriteOnly);
        if(proFile->isOpen())
        {
          proFile->write(pro.toUtf8());
        }
        proFile->close();
        delete proFile;


    }

    done(QDialog::Accepted);

}

NewActorDialog::~NewActorDialog()
{
    delete ui;
}
