#include "nodematerial.h"
#include "src/qnodeseditor/qneport.h"
#include <iostream>
#include "src/utils.h"
#include "src/Assets/assetscollections.h"
#include "src/Editor/Utils/foldergestion.h"

std::map<std::string,int> NodeMaterial::occurence;
std::vector<int> NodeMaterial::bindingAvailable;


NodeMaterial::NodeMaterial()
{


}

NodeMaterial::NodeMaterial(QGraphicsScene *scene,QNEBlock *b,NodeBase* nodeBase)
{
    m_b = b;
    m_scene = scene;
    connect(this,SIGNAL(onFragmentSourceChanged(QString)),nodeBase,SLOT(buildShader()));
}

NodeMaterial::NodeMaterial(QPoint pos, QGraphicsScene *scene, GlslTreeWidgetItem *item,NodeBase* nodeBase)
{

    m_isUniform = item->getIsUniform();
    /*Init Fragment Shader*/
    if(m_isUniform)
        m_fragmentSource = "";
    else
        m_fragmentSource = "    ";
    m_fragmentSource += item->getCode();
    m_fragmentSourceOld = m_fragmentSource;
    m_name = item->getName();
    m_type = item->getType();
    m_inputs = item->getInputs();
    m_outputs = item->getOutputs();
    m_outputsType = item->getOutputsType();
    m_isVariable = item->getIsVariable();
    m_hasImage = item->getHasImage();
    m_nbParams = item->getNbParams();
    m_scene = scene;
    m_b = new QNEBlock(100,50,0,this,NULL);
    if(m_hasImage)
        m_b->setSize(150,150);
    m_b->setPos(pos);
    m_scene->addItem(m_b);


    m_b->addPort(m_name, 0,this,NULL,-1, QNEPort::NamePort);
    m_PortType = m_b->addPort(m_type, 0,this,NULL,-1, QNEPort::TypePort);

    inputs = m_b->addInputPorts(m_inputs,this,NULL);

    outputs = m_b->addOutputPorts(m_outputs,this,NULL);

    foreach(QNEPort* port,inputs){
        connect(port,SIGNAL(haveBeenConnected(QNEPort*,QNEPort*)),this,SLOT(oneInputPortHaveBeenConnect(QNEPort*,QNEPort*)));
        connect(port,SIGNAL(haveBeenDisconnected(QNEPort*,QNEPort*)),this,SLOT(onePortHaveBeenDisconnect(QNEPort*,QNEPort*)));
    }

    foreach(QNEPort* port,outputs){
        connect(port,SIGNAL(haveBeenConnected(QNEPort*,QNEPort*)),this,SLOT(oneInputPortHaveBeenConnect(QNEPort*,QNEPort*)));
        connect(port,SIGNAL(haveBeenDisconnected(QNEPort*,QNEPort*)),this,SLOT(onePortHaveBeenDisconnect(QNEPort*,QNEPort*)));
    }
    while(occurence["binding"] <3)
        occurence["binding"]++;
    int binding = -1;
    if(m_hasImage){
        if(bindingAvailable.size()>0){
            binding = bindingAvailable.at(0);
            bindingAvailable.erase(bindingAvailable.begin());
        }else{
            binding = occurence["binding"]++;
        }
    }

    size_t posStr = m_fragmentSourceOld.toStdString().find("//#numberOccurance");
    m_varName = m_name + "_" + QString::number(occurence[m_name.toStdString()]);
    QString varNameAndType;
    m_binding = binding;
    m_isAbstract = 0;
    if(m_isUniform)
        if(m_hasImage)
            varNameAndType = "layout(binding = " + QString::number(binding) + ") uniform " + m_outputsType + " " + m_varName;
        else
            varNameAndType = "uniform " + m_outputsType + " " + m_varName;
    else
        if(m_outputsType.compare("abstract") == 0){
            varNameAndType = "//#varType " + m_varName;
            m_isAbstract = 1;
        }
        else if(m_outputsType.compare("abstractPlus") == 0){
            varNameAndType = "//#varType " + m_varName;
            m_isAbstract = 2;
        }
        else
            varNameAndType = m_outputsType + " " + m_varName;
    m_fragmentSource.replace(posStr,QString("//#numberOccurance").length(),varNameAndType);
    m_fragmentSource += "\n";
    m_fragmentSourceOld = m_fragmentSource;
    occurence[m_name.toStdString()] ++;

    for(int i=0;i<m_nbParams;i++){
        m_value.push_back(new Param(0,false));
        connect(m_value.at(i),SIGNAL(paramHaveBeenChanged(float,bool)),this,SLOT(paramChanged()));
    }

    m_imageLoad = false;
    if(m_isAbstract)
        m_returnType = Float;
    else
        m_returnType = Utils::stringToTypeVar(m_outputsType);
    connect(this,SIGNAL(onFragmentSourceChanged(QString)),nodeBase,SLOT(buildShader()));

    posStr = m_fragmentSource.toStdString().find("//#varName");
    while(std::string::npos != posStr){
        m_fragmentSource.replace(posStr,QString("//#varName").length(),m_varName);
        posStr = m_fragmentSource.toStdString().find("//#varName");
    }
}



NodeMaterial::~NodeMaterial()
{

}

QString NodeMaterial::getFragmentSource()
{
    return m_fragmentSource;
}

QString NodeMaterial::getName()
{
    return m_name;
}

bool NodeMaterial::isVariable()
{
    return m_isVariable;
}

bool NodeMaterial::haveImage()
{
    return m_hasImage;
}

int NodeMaterial::getNbParams()
{
    return m_nbParams;
}

bool NodeMaterial::isUniform()
{
    return m_isUniform;
}

QNEBlock *NodeMaterial::getQNEBlock()
{
    return m_b;
}

Param *NodeMaterial::getParam(int i)
{
    return m_value.at(i);
}

QString NodeMaterial::getVarName()
{
    return m_varName;
}

std::vector<NodeMaterial *> NodeMaterial::getParents()
{
    return m_parents;
}

QString NodeMaterial::getImagePath()
{
    return m_imagePath;
}

int NodeMaterial::getBinding()
{
    return m_binding;
}

void NodeMaterial::freeBinding()
{
    if(m_binding!=-1){
        bindingAvailable.push_back(m_binding);
    }
    m_binding = -1;
}

void NodeMaterial::setTypeVar()
{

    size_t posStr = m_fragmentSource.toStdString().find("//#varType");

    if(posStr!=std::string::npos){
        m_fragmentSource.replace(posStr,QString("//#varType").length(),Utils::typeVarToString(m_returnType));
    }
}

bool NodeMaterial::isAbstract()
{
    return m_isAbstract;
}

void NodeMaterial::setDefaultValue()
{
    QString string;
    size_t posStr = m_fragmentSource.toStdString().find("//#value");
    if(posStr != std::string::npos){
        m_fragmentSource = m_fragmentSourceOld;
        QString newValueStr = m_name + "(";
        for(int i=0;i<m_nbParams;i++){
            newValueStr += QString::number(0);
            if(i>m_nbParams-1)
                newValueStr += ",";
            string += QString::number(0);
            string += " ";
        }
        newValueStr += ")";
        if(m_nbParams==1){
            if(m_name.compare("bool")==0){
                newValueStr = "false";
                string = newValueStr;
            }
            else
                newValueStr = QString::number(0);
        }

        m_fragmentSource.replace(posStr,QString("//#value").length(),newValueStr);
    }
}

QString NodeMaterial::getOutputType()
{
    return m_outputsType;
}

TypeVarType NodeMaterial::getTypeVar()
{
    return m_returnType;
}

void NodeMaterial::save(QDataStream &ds)
{


    ds << m_name;
    ds << m_type;
    ds << m_inputs;
    ds << m_outputs;
    ds << m_outputsType;
    ds << m_isVariable;
    ds << m_hasImage;
    ds << m_isUniform;

    ds << m_fragmentSource;
    ds << m_fragmentSourceOld;

    ds << m_varName;
    ds << m_nbParams;

    foreach(Param* value,m_value){
       ds << value->value;
       ds << value->b;
    }

    ds << m_imageLoad;
    ds << m_binding;
    ds << m_isAbstract;
    QString copy;
    if(m_hasImage){
        copy = FolderGestion::removeProjectPath(FolderGestion::checkoutReferences(m_imagePath));
    }
    else
        copy = m_imagePath;
    ds << copy;

}

void NodeMaterial::load(QDataStream &ds)
{
    ds >> m_name;
    ds >> m_type;
    ds >> m_inputs;
    ds >> m_outputs;
    ds >> m_outputsType;
    ds >> m_isVariable;
    ds >> m_hasImage;
    ds >> m_isUniform;

    ds >> m_fragmentSource;
    ds >> m_fragmentSourceOld;

    ds >> m_varName;
    ds >> m_nbParams;

    for(int i=0;i<m_nbParams;i++){
       float value;
       bool b;
       ds >> value;
       ds >> b;
       m_value.push_back(new Param(value,b));
       connect(m_value.at(i),SIGNAL(paramHaveBeenChanged(float,bool)),this,SLOT(paramChanged(float,bool)));
    }

    ds >> m_imageLoad;
    ds >> m_binding;
    ds >> m_isAbstract;
    ds >> m_imagePath;
    if(m_hasImage && m_imageLoad){
        m_imagePath = FolderGestion::checkoutReferences(m_imagePath);
    }


    while(occurence["binding"] <3)
        occurence["binding"]++;
    int binding = -1;
    if(m_hasImage){
        if(bindingAvailable.size()>0){
            binding = bindingAvailable.at(0);
            bindingAvailable.erase(bindingAvailable.begin());
        }else{
            binding = occurence["binding"]++;
        }
    }

    if(m_hasImage && m_imageLoad){
        m_b->setSize(150,150);
        unsigned int handle = AssetsCollections::addTexture(m_imagePath.toStdString(),m_imagePath.toStdString());
        if(handle!=-1){
            QSize myRessortSize(100,100);
            QPixmap myNewRessort = AssetsCollections::TexturesCollection[handle]->getThumnail().scaled(myRessortSize,Qt::KeepAspectRatio,Qt::SmoothTransformation);

            m_item = m_scene->addPixmap(myNewRessort);

            m_item->setPos(-100/2 -5,-25);

            m_item->setParentItem((QGraphicsItem*)m_b);
        }
    }

    occurence[m_name.toStdString()] ++;

    m_returnType = Utils::stringToTypeVar(m_outputsType);
    emit onFragmentSourceChanged(m_fragmentSource);

}

void NodeMaterial::addPort(QNEPort *port)
{
    connect(port,SIGNAL(haveBeenConnected(QNEPort*,QNEPort*)),this,SLOT(oneInputPortHaveBeenConnect(QNEPort*,QNEPort*)));
    connect(port,SIGNAL(haveBeenDisconnected(QNEPort*,QNEPort*)),this,SLOT(onePortHaveBeenDisconnect(QNEPort*,QNEPort*)));
    if(port->portFlags() == QNEPort::TypePort){
        m_PortType = port;
    }
    else if(port->isOutput())
        outputs.push_back(port);
    else
        inputs.push_back(port);

}


void NodeMaterial::oneInputPortHaveBeenConnect(QNEPort *in ,QNEPort *out)
{
    size_t posStr;
    if(in->getNode() == this){
        m_parents.push_back(out->getNode());

        QString strToReplace = "//#input" + QString::number(in->number());
        posStr = m_fragmentSource.toStdString().find(strToReplace.toStdString());
        while(std::string::npos != posStr){
            m_fragmentSource.replace(posStr,strToReplace.length(),out->getNode()->getVarName());
            posStr = m_fragmentSource.toStdString().find(strToReplace.toStdString());
        }


        if(m_isAbstract == 1){
            TypeVarType tempT = Utils::stringToTypeVar(out->getNode()->getOutputType());
            if(tempT>m_returnType){
                m_returnType = Utils::stringToTypeVar(out->getNode()->getOutputType());
                m_outputsType = out->getNode()->getOutputType();
            }
            posStr = m_fragmentSource.toStdString().find("//#input");
            if(std::string::npos == posStr)
                setTypeVar();
        }else if(m_isAbstract == 2){
            bool connected = false;
            foreach(QNEPort *in,inputs){
                if(in->isConnected()){
                    connected = true;
                    break;
                }
            }
            if(!connected){


                m_returnType = out->getNode()->getTypeVar();
                m_outputsType = out->getNode()->getOutputType();

            }
            else{
                TypeVarType tempT = Utils::addTypeVar(m_returnType, out->getNode()->getTypeVar());

                m_returnType = tempT;
                m_outputsType = Utils::typeVarToString(tempT);
                posStr = m_fragmentSource.toStdString().find("//#value");
                if(std::string::npos == posStr)
                    setTypeVar();

            }
        }

        emit onFragmentSourceChanged(m_fragmentSource);
    }

}

void NodeMaterial::onePortHaveBeenDisconnect(QNEPort *in, QNEPort *out)
{
    std::vector<NodeMaterial*>::iterator it;
    if(in->getNode() == this){


        if(m_isAbstract){
            std::vector<std::string> list = Utils::split(m_fragmentSource.toStdString(),' ');
            int i=0;
            while(i<list.size() && list.at(i).size()==0)
                i++;
            int posStr2 = m_fragmentSource.toStdString().find(list.at(i));//looking for vartype

            m_fragmentSource.replace(posStr2,QString(list.at(i).c_str()).length(),"//#varType");
        }

        it = find (m_parents.begin(), m_parents.end(), out->getNode());
        QString strToReplace = out->getNode()->getVarName();
        QString replaceBy = "//#input" + QString::number(in->number());
        int posStr = m_fragmentSource.toStdString().find(strToReplace.toStdString());

        m_fragmentSource.replace(posStr,strToReplace.length(),replaceBy);
        if (it != m_parents.end())
            m_parents.erase(it);

        if(m_isAbstract==1){
            TypeVarType tempT = Float;
            foreach(NodeMaterial *n,m_parents){
                if(tempT<n->getTypeVar()){
                    tempT = n->getTypeVar();
                }
            }
            m_returnType = tempT;
            m_outputsType = Utils::typeVarToString(tempT);
        }else if(m_isAbstract==2){
            TypeVarType tempT = Float;
            int j=0;
            foreach(NodeMaterial *n,m_parents){
                    if(j==0)
                        tempT = n->getTypeVar();
                    else
                        tempT = Utils::addTypeVar(tempT,n->getTypeVar());
                    j++;

            }
            m_returnType = tempT;
            m_outputsType = Utils::typeVarToString(tempT);
        }
        emit onFragmentSourceChanged(m_fragmentSource);
    }
    if(out->getNode() == this){

    }

}

void NodeMaterial::paramChanged()
{

    QString string;
    size_t posStr = m_fragmentSourceOld.toStdString().find("//#value");
    m_fragmentSource = m_fragmentSourceOld;
    QString newValueStr = m_name + "(";
    for(int i=0;i<m_nbParams;i++){
        newValueStr += QString::number(m_value.at(i)->value);
        if(i<m_nbParams-1)
            newValueStr += ",";
        string += QString::number(m_value.at(i)->value);
        string += " ";
    }
    newValueStr += ")";
    if(m_nbParams==1){
        if(m_name.compare("bool")==0){
            newValueStr = (m_value.at(0)->b)?"true":"false";
            string = newValueStr;
        }
        else
            newValueStr = QString::number(m_value.at(0)->value);
    }
    m_PortType->setName(string);



    m_fragmentSource.replace(posStr,QString("//#value").length(),newValueStr);
    posStr = m_fragmentSource.toStdString().find("//#value");
    if(std::string::npos == posStr && m_isAbstract)
        setTypeVar();
    emit onFragmentSourceChanged(m_fragmentSource);
}

void NodeMaterial::changeImage(Texture* texture)
{

    m_imagePath = QString(texture->GetPath().c_str());

    QSize myRessortSize(100,100);
    QPixmap myNewRessort = texture->getThumnail().scaled(myRessortSize,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    if(m_imageLoad)
        m_scene->removeItem((QGraphicsItem*)m_item);
    m_item = m_scene->addPixmap(myNewRessort);

    m_item->setPos(-100/2 -5,-25);

    m_item->setParentItem((QGraphicsItem*)m_b);
    m_imageLoad = true;
    emit onFragmentSourceChanged(m_fragmentSource);
}
