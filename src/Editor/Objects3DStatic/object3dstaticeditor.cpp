#include "object3dstaticeditor.h"
#include "ui_object3dstaticeditor.h"
#include "src/Assets/assetscollections.h"
#include "src/Engine/Models/model.h"

#include <QFileDialog>
#include "src/Editor/Utils/foldergestion.h"

#include <iostream>

Object3DStaticEditor::Object3DStaticEditor(QWidget * parent) : QMainWindow(parent),
    ui(new Ui::Object3DStaticEditor)
{
    ui->setupUi(this);
    ui->centralwidget->setLayout(ui->gridLayout);

}

void Object3DStaticEditor::init(QString path)
{
    if (ui->layoutElements) {
        while(ui->layoutElements->count() > 0){
            QLayoutItem *item = ui->layoutElements->takeAt(0);
            QWidget* widget = item->widget();
            if(widget)
                delete widget;
            delete item;
        }
    }
    std::replace( path.begin(), path.end(), '/', '\\');
    std::vector<std::string> split = Utils::split(path.toStdString(),'\\');

    //AssetsCollections::addObject3D(path.toStdString(),path.toStdString());*/
    unsigned int handle = ui->objectGLWidget->setObjectToRender(split.at(split.size()-1),path.toStdString());
    m_handleObject = handle;

    std::vector<Model*> models = AssetsCollections::Object3DStaticCollection.GetElement(handle)->getModels();

    for(int i=0;i<models.size();i++){
        std::cout << models.at(i)->getMesh().getVaoID() << std::endl;
        QLabel *label =  new QLabel();
        label->setText(QString::number(models.at(i)->getMesh().getVaoID()));
        MatButton *matButton = new MatButton();
        matButton->setText("pickMat");
        matButton->id = i;
        matButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
        ui->layoutElements->insertRow(i,label,matButton);
        connect(matButton,SIGNAL(clicked(MatButton*)),this,SLOT(setMaterial(MatButton*)));
    }
}

void Object3DStaticEditor::closeEvent(QCloseEvent *event)
{
    ui->objectGLWidget->close();

}

Object3DStaticEditor::~Object3DStaticEditor()
{
    delete ui;
}

void Object3DStaticEditor::setMaterial(MatButton* button)
{
    QString fileName =
                QFileDialog::getOpenFileName(nullptr,
                                             QObject::tr("Open Material files"),
                                             QString(FolderGestion::rootProjectsFolderPath),
                                             QObject::tr("Material file (*.smat)"));

    if(fileName.at(0)!=0){
        std::replace( fileName.begin(), fileName.end(), '/', '\\');
        std::vector<std::string> split = Utils::split(fileName.toStdString(),'\\');
        unsigned int handle = AssetsCollections::MaterialsCollection.Add(split.at(split.size()-1),fileName.toStdString());
        if(handle!=-1){
            AssetsCollections::HandlesMaterials.push_back(handle);

            AssetsCollections::Object3DStaticCollection.GetElement(m_handleObject)->getModel(button->id)->setMaterial(AssetsCollections::MaterialsCollection.GetElement(handle));
            button->setText(QString(split.at(split.size()-1).c_str()));
        }
    }
}
