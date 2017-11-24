#include "ui_materialeditorwindow.h"
#include "src/Editor/Materials/materialeditorwindow.h"
#include "src/Editor/Materials/materialgraphwidget.h"
#include "src/Editor/Materials/customlabeltype.h"
#include <iostream>
#include <QDoubleSpinBox>
#include <QDebug>
#include <QCheckBox>
#include <QLabel>
#include <limits>
#include <QColorDialog>
#include "src/Engine/Materials/texture.h"


MaterialEditorWindow::MaterialEditorWindow( QWidget * parent) : QMainWindow(parent),
    ui(new Ui::MaterialEditorWindow)
{
    ui->setupUi(this);
    ui->centralwidget->setLayout(ui->gridLayout);
    QGridLayout* layout = new QGridLayout();
    ui->Details->setLayout(layout);

    connect(ui->MaterialView->getNodeBase(),SIGNAL(onFragmentSourceChanged(QString)),ui->DebugBrowser,SLOT(setText(QString)));
    connect(ui->sphereButton,SIGNAL(clicked(bool)),ui->openGLWidget,SLOT(switchToSphereSlot()));
    connect(ui->cubeButton,SIGNAL(clicked(bool)),ui->openGLWidget,SLOT(switchToCubeSlot()));
    connect(ui->planeButton,SIGNAL(clicked(bool)),ui->openGLWidget,SLOT(switchToPlaneSlot()));
    connect(ui->cylinderButton,SIGNAL(clicked(bool)),ui->openGLWidget,SLOT(switchToCylinderSlot()));
    connect(ui->customButton,SIGNAL(clicked(bool)),ui->openGLWidget,SLOT(switchToCustomSlot()));
    connect(ui->lightAutoRotatecheckBox,SIGNAL(clicked(bool)),ui->openGLWidget,SLOT(lightAutoRotate(bool)));
    connect(ui->lightColorButton,SIGNAL(clicked(bool)),this,SLOT(pickColor()));
    connect(this,SIGNAL(sendColor(glm::vec3)),ui->openGLWidget,SLOT(setLightColor(glm::vec3)));

    /*Connect save and load*/
    connect(ui->actionSave,SIGNAL(triggered(bool)),ui->MaterialView,SLOT(saveMaterial()));


    CustomLabelType* label = new CustomLabelType("",true);

    ui->PreviewSettings->addWidget(label);
    connect(label,SIGNAL(clicked(Texture*)),ui->openGLWidget,SLOT(changeHDRI(Texture*)));

}

MaterialEditorWindow::~MaterialEditorWindow()
{
    SAFE_DELETE(ui);
}

MaterialGraphWidget *MaterialEditorWindow::getMaterialView()
{
    return ui->MaterialView;
}

GLMaterialPreview *MaterialEditorWindow::getGlPreview()
{
    return ui->openGLWidget;
}

QTextBrowser *MaterialEditorWindow::getDebugBrowser()
{
    return ui->DebugBrowser;
}

QString MaterialEditorWindow::getPathMaterial()
{
    return m_pathMaterial;
}

void MaterialEditorWindow::closeEvent(QCloseEvent *event)
{
    ui->openGLWidget->close();
}

void MaterialEditorWindow::init(QString path)
{
    m_pathMaterial = path;
    ui->MaterialView->setMaterialPath(m_pathMaterial);
}

void MaterialEditorWindow::nodeSelected(NodeMaterial * node)
{
    QLayoutItem* child;
    while ((child = ui->Details->layout()->takeAt(0)) != 0)
    {
        if (child->widget() != NULL)
        {
            delete (child->widget());
        }
        delete child;
    }

    for(int i=0;i<node->getNbParams();i++){
        if(node->getName().compare("bool") == 0){
            QCheckBox* check = new QCheckBox();
            check->setChecked(node->getParam(i)->b);
            ui->Details->layout()->addWidget(check);
            connect(check,SIGNAL(clicked(bool)),node->getParam(i),SLOT(changeBool(bool)));
        }else{
            QDoubleSpinBox* spin = new QDoubleSpinBox();
            spin->setMaximum(std::numeric_limits<float>::max());
            spin->setMinimum(-std::numeric_limits<float>::max());
            spin->setDecimals(5);
            spin->setValue(node->getParam(i)->value);
            ui->Details->layout()->addWidget(spin);
            connect(spin,SIGNAL(valueChanged(double)),node->getParam(i),SLOT(changeParam(double)));
        }
    }
    if(node->haveImage()){
        CustomLabelType* label = new CustomLabelType(node->getImagePath(),false);

        ui->Details->layout()->addWidget(label);
        connect(label,SIGNAL(clicked(Texture*)),node,SLOT(changeImage(Texture*)));
    }

}

void MaterialEditorWindow::pickColor()
{
    QColorDialog *colorPick = new QColorDialog();
    colorPick->exec();
    QColor color = colorPick->selectedColor();
    if(color.isValid()) {

        ui->lightColorButton->setText(color.name());

        QString qss = QString("background-color: %1").arg(color.name());
        ui->lightColorButton->setStyleSheet(qss);


        emit sendColor(glm::vec3(color.red()/255.0,color.green()/255.0,color.blue()/255.0));
    }
}

