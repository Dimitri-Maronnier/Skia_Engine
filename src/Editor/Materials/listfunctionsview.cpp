#include "listfunctionsview.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <iostream>
#include <string>
#include <QTreeWidgetItem>
#include "src/Editor/Materials/glsltreewidgetitem.h"
#include "src/utils.h"


ListFunctionsView::ListFunctionsView(QWidget *parent)
    : QTreeWidget(parent)
{
    QString QfileName = ":/Shaders/Utils/glslOverall";
    QFile fichier(QfileName);

    fichier.open(QIODevice::ReadOnly);
    QTextStream flux(&fichier);

    if(!fichier.isOpen()){
        qDebug() << "Cannot open " << QfileName;
    }

    QString ligne;
    while(! flux.atEnd())
    {
        ligne = flux.readLine();
        if(ligne.size() > 0 && ligne.at(0) == '['){
            std::string all = Utils::stringBetween(ligne.toStdString(),'[',']');

            std::vector<std::string> lineSplit = Utils::split(all,',');

            /*get category*/
            std::vector<std::string> subSplit = Utils::split(lineSplit.at(0),'=');
            QString category = QString(subSplit.at(1).c_str());


            /*get name*/
            subSplit = Utils::split(lineSplit.at(1),'=');
            QString name = QString(subSplit.at(1).c_str());

            /*get type*/
            subSplit = Utils::split(lineSplit.at(2),'=');
            QString type = QString(subSplit.at(1).c_str());

            /*get input*/
            std::string allinput = Utils::stringBetween(lineSplit.at(3),'{','}');
            subSplit = Utils::split(allinput,';');
            QStringList inputs;
            foreach(std::string str,subSplit)
                inputs << str.c_str();

            /*get output*/
            std::string alloutput = Utils::stringBetween(lineSplit.at(4),'{','}');
            subSplit = Utils::split(alloutput,';');
            QStringList outputs;
            foreach(std::string str,subSplit)
                outputs << str.c_str();

            /*get HaveImage*/
            subSplit = Utils::split(lineSplit.at(5),'=');
            bool hasImage = (subSplit.at(1).compare("true") == 0 );

            /*get IsVariable*/
            subSplit = Utils::split(lineSplit.at(6),'=');
            bool isVariable = (subSplit.at(1).compare("true") == 0 );

            /*get NbParams*/
            subSplit = Utils::split(lineSplit.at(7),'=');
            int nbParams = atoi(subSplit.at(1).c_str());

            /*get output type*/
            subSplit = Utils::split(lineSplit.at(8),'=');
            QString outputtype = QString(subSplit.at(1).c_str());

            /*get IsUniform*/
            subSplit = Utils::split(lineSplit.at(9),'=');
            bool isUniform = (subSplit.at(1).compare("true") == 0 );

            /*get Code*/
            std::string code = Utils::stringBetween(ligne.toStdString(),'"','"');

            GlslTreeWidgetItem *item = new GlslTreeWidgetItem(name,type,inputs,outputs,hasImage,isVariable,nbParams,outputtype,isUniform,QString(code.c_str()));
            item->setText(0,name);
            int i=0;
            while(i<this->topLevelItemCount() && this->topLevelItem(i)->text(0).compare(category)!=0)
                i++;
            if(i==this->topLevelItemCount()){
                QTreeWidgetItem* topItem = new QTreeWidgetItem();
                topItem->setText(0,category);
                topItem->setFlags( topItem->flags() ^ Qt::ItemIsDragEnabled );
                this->addTopLevelItem(topItem);
                topItem->addChild(item);
            }else{
                this->topLevelItem(i)->addChild(item);
            }
            this->sortByColumn(0,Qt::SortOrder::AscendingOrder);
            this->expandAll();
        }
    }

}

ListFunctionsView::~ListFunctionsView()
{

}
