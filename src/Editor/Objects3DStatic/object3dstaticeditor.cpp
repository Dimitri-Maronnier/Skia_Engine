#include "object3dstaticeditor.h"
#include "ui_object3dstaticeditor.h"
#include "src/Assets/assetscollections.h"
#include "src/Engine/Models/model.h"

#include <QFileDialog>
#include "src/Editor/Utils/foldergestion.h"
#include "src/Editor/Utils/compressor.h"
#include "src/Editor/Utils/foldergestion.h"
#include "src/utils.h"

#include <iostream>

Object3DStaticEditor::Object3DStaticEditor(QWidget * parent) : QMainWindow(parent),
    ui(new Ui::Object3DStaticEditor)
{
    ui->setupUi(this);


}

void Object3DStaticEditor::init(QString path)
{
    m_path = path;
    if (ui->layoutElements) {
        while(ui->layoutElements->count() > 0){
            QLayoutItem *item = ui->layoutElements->takeAt(0);
            QWidget* widget = item->widget();
            if(widget)
                delete widget;
            delete item;
        }
    }

    connect(ui->actionSave,SIGNAL(triggered(bool)),this,SLOT(saveObject()));
    connect(ui->Radius,SIGNAL(valueChanged(double)),this,SLOT(setRadius(double)));
    connect(ui->Tag,SIGNAL(valueChanged(int)),this,SLOT(setTag(int)));


    std::replace( path.begin(), path.end(), '/', '\\');
    std::vector<std::string> split = Utils::split(path.toStdString(),'\\');
    std::string name = Utils::removeExtension(QString(split.at(split.size()-1).c_str())).toStdString();


    unsigned int handle = AssetsCollections::Object3DStaticCollection.Add(name,path.toStdString());
    AssetsCollections::HandlesObject3DStatic.push_back(handle);
    //m_object = AssetsCollections::Object3DStaticCollection.GetElement(handle);
    m_handleObject = handle;

    m_materialsPath = AssetsCollections::Object3DStaticCollection.GetElement(handle)->getMaterialsPath();

    std::vector<Model*> models = AssetsCollections::Object3DStaticCollection.GetElement(handle)->getModels();

    ui->Radius->setValue(AssetsCollections::Object3DStaticCollection.GetElement(handle)->getRadius());

    ui->Tag->setValue(AssetsCollections::Object3DStaticCollection.GetElement(handle)->getTag());

    m_models_count = models.size();

    for(int i=0;i<models.size();i++){
        QLabel *label =  new QLabel();
        label->setText(QString::number(i));
        MatButton *matButton = new MatButton();
        if(m_materialsPath->size()>0){
            std::replace( m_materialsPath->at(i).begin(), m_materialsPath->at(i).end(), '/', '\\');
            std::vector<std::string> split = Utils::split(m_materialsPath->at(i).toStdString(),'\\');
            matButton->setText(( m_materialsPath->at(i) != "")?QString(split.at(split.size()-1).c_str()):"pickMat");
        }
        else{
            matButton->setText("pickMat");
        }
        matButton->id = i;
        matButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
        ui->layoutElements->insertRow(i,label,matButton);
        connect(matButton,SIGNAL(clicked(MatButton*)),this,SLOT(setMaterial(MatButton*)));
    }

}

void Object3DStaticEditor::closeEvent(QCloseEvent *event)
{
    //ui->objectGLWidget->close();

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
                                             QString(FolderGestion::currentWorkingDir.c_str()),
                                             QObject::tr("Material file (*.smat)"));

    if(fileName.at(0)!=0){
        std::replace( fileName.begin(), fileName.end(), '/', '\\');
        std::vector<std::string> split = Utils::split(fileName.toStdString(),'\\');
        unsigned int handle = AssetsCollections::MaterialsCollection.Add(split.at(split.size()-1),fileName.toStdString());
        if(handle!=-1){
            AssetsCollections::HandlesMaterials.push_back(handle);

            AssetsCollections::Object3DStaticCollection.GetElement(m_handleObject)->getModel(button->id)->setMaterial(AssetsCollections::MaterialsCollection.GetElement(handle));
            m_materialsPath->at(button->id) = fileName;

            button->setText(QString(split.at(split.size()-1).c_str()));
        }
    }
}

void Object3DStaticEditor::saveObject()
{

    QFile f(m_path);
    f.open(QFile::ReadOnly);
    QDataStream dataStream(&f);

    unsigned int nomberMeshs;
    dataStream >> nomberMeshs;

    std::vector<unsigned int> nomberFaces,nomberVertices,nomberTextures;
    std::vector<QString> names;
    std::vector<std::vector<unsigned int>> faceArrayTotal;
    std::vector<std::vector<float>> positionArrayTotal,normalsArrayTotal,texturesArrayTotal,tangentsArrayTotal;

    for(unsigned int numMesh=0;numMesh<nomberMeshs;numMesh++){

        QString name;
        std::vector<unsigned int> faceArray;
        std::vector<float> positionArray,normalsArray,texturesArray,tangentsArray;


        //Reading name
        dataStream >> name;
        names.push_back(name);

        unsigned int nomberFace,nomberVertice,nomberTexture;

        //Reading size
        dataStream >> nomberFace;
        dataStream >> nomberVertice;
        dataStream >> nomberTexture;

        nomberFaces.push_back(nomberFace);
        nomberVertices.push_back(nomberVertice);
        nomberTextures.push_back(nomberTexture);


        //Read face Array
        for(unsigned int i=0;i<nomberFace;i++){
            unsigned int face;
            dataStream >> face;
            faceArray.push_back(face);
        }

        //Read positions array
        for(unsigned int i=0;i<nomberVertice ;i++){
            float position;
            dataStream >> position;
            positionArray.push_back(position);
        }

        //Read normals array
        for(int i=0;i<nomberVertice ;i++){
            float normal;
            dataStream >> normal;
            normalsArray.push_back(normal);
        }

        //Read texture array
        for(unsigned int i=0;i<nomberTexture;i++){
            float texture;
            dataStream >> texture;
            texturesArray.push_back(texture);
        }
        //Read tangent array
        for(unsigned int i=0;i<nomberVertice;i++){
            float tangent;
            dataStream >> tangent;
            tangentsArray.push_back(tangent);
        }

        faceArrayTotal.push_back(faceArray);
        positionArrayTotal.push_back(positionArray);
        normalsArrayTotal.push_back(normalsArray);
        texturesArrayTotal.push_back(texturesArray);
        tangentsArrayTotal.push_back(tangentsArray);
    }


    f.close();
    QFile fwrite(m_path);
    fwrite.open(QFile::WriteOnly);
    QDataStream dsWrite(&fwrite);
    dsWrite << nomberMeshs;

    for(unsigned int numMesh=0;numMesh<nomberMeshs;numMesh++){

        //write name
        dsWrite << names.at(numMesh);

        //write size
        dsWrite << nomberFaces.at(numMesh);
        dsWrite << nomberVertices.at(numMesh);
        dsWrite << nomberTextures.at(numMesh);

        //write face Array
        for(unsigned int i=0;i<nomberFaces.at(numMesh);i++){
            dsWrite << faceArrayTotal.at(numMesh).at(i);
        }

        //write positions array
        for(unsigned int i=0;i<nomberVertices.at(numMesh) ;i++){
            dsWrite << positionArrayTotal.at(numMesh).at(i);
        }

        //write normals array
        for(unsigned int i=0;i<nomberVertices.at(numMesh) ;i++){
            dsWrite << normalsArrayTotal.at(numMesh).at(i);
        }

        //write texture array
        for(int i=0;i<nomberTextures.at(numMesh);i++){
            dsWrite << texturesArrayTotal.at(numMesh).at(i);
        }
        //write tangent array
        for(unsigned int i=0;i<nomberVertices.at(numMesh);i++){
            dsWrite << tangentsArrayTotal.at(numMesh).at(i);
        }
    }

    dsWrite << "Properties";
    dsWrite << m_models_count;
    foreach(QString path,*m_materialsPath){
        if(path.size()>0)
            dsWrite << FolderGestion::removeProjectPath(FolderGestion::checkoutReferences(path)).toStdString().c_str();
        else
            dsWrite << "";

    }
    dsWrite << AssetsCollections::Object3DStaticCollection.GetElement(m_handleObject)->getRadius();
    dsWrite << AssetsCollections::Object3DStaticCollection.GetElement(m_handleObject)->getTag();
    fwrite.close();

}

void Object3DStaticEditor::setRadius(double f)
{
    AssetsCollections::Object3DStaticCollection.GetElement(m_handleObject)->setRadius(f);
}

void Object3DStaticEditor::setTag(int index)
{
    AssetsCollections::Object3DStaticCollection.GetElement(m_handleObject)->setTag(index);
}
