#include "newprojectdialog.h"
#include "ui_newprojectdialog.h"
#include "src/Editor/Utils/foldergestion.h"
#include <iostream>
#include <QLineEdit>
#include "src/Editor/Utils/projectinfo.h"

NewProjectDialog::NewProjectDialog( QWidget * parent) : QDialog(parent),
    ui(new Ui::NewProjectDialog)
{
    ui->setupUi(this);
}

void NewProjectDialog::accept(){


    if(FolderGestion::createProjectFolder(ui->name->text().toStdString()) == -1)
        std::cout << "Project already exists" << std::endl;
    ProjectInfo::name = ui->name->text().toStdString();
    done(QDialog::Accepted);

}

NewProjectDialog::~NewProjectDialog()
{
    delete ui;
}
