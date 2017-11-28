#include "src/Editor/Materials/Nodes/nodebase.h"
#include "src/qnodeseditor/qneport.h"
#include <iostream>
#include "src/Engine/Materials/material.h"
#include "src/Editor/Utils/foldergestion.h"

NodeBase::NodeBase()
{

}

NodeBase::NodeBase(QNEBlock *b)
{
    m_b = b;
}

NodeBase::NodeBase(QPoint pos, QGraphicsScene *scene)
{
    /*Init Fragment Shader*/
    /*m_fragmentSource = "#version 430 core\n\n"
                       "in VS_OUT{\n"
                        "   vec3 normal;\n"
                        "   vec3 toLightVector;\n"
                        "   vec3 toLightVectorTangent;\n"
                        "   vec3 toCameraVector;\n"
                        "   vec3 toCameraVectorTangent;\n"
                        "   vec2 textureCoord;\n"
                        "   vec3 viewDir;\n"
                        "}fs_in;\n\n"
                        "out vec4 fragmentColor;\n\n"
                        "uniform vec3 lightTint;\n\n"
                        "//#toReplaceUniform\n\n"
                        "//#toReplaceFunctions\n\n"
                       "void main(void){\n"
                       "    bool normalMapped = false;\n"
                       "    vec4 BaseColor=vec4(1);\n"
                       "    vec3 reflectivity = vec3(1);\n"
                       "    float shineDamper=10;\n"
                       "    vec4 normalValue = vec4(0);\n"
                       "//#toReplace\n"
                       "    vec3 unitLightVector;\n"
                       "    vec3 unitVectorToCamera;\n"
                       "    vec3 unitNormal;\n"
                       "    if(normalMapped){\n"
                       "        unitLightVector = normalize(fs_in.toLightVectorTangent);\n"
                       "        unitVectorToCamera = normalize(fs_in.toCameraVectorTangent);\n"
                       "        unitNormal = normalize(normalValue.rgb);\n"
                       "    }else{\n"
                       "        unitLightVector = normalize(fs_in.toLightVector);\n"
                       "        unitVectorToCamera = normalize(fs_in.toCameraVector);\n"
                       "        unitNormal = normalize(fs_in.normal);\n"
                       "    }\n"
                       "    float nDotl = dot(unitNormal,unitLightVector);\n"
                       "    float brightness = max(nDotl,0.2);\n"
                       "    vec3 lightDirection = -unitLightVector;\n"
                       "    vec3 reflectedLightDirection = reflect(lightDirection,unitNormal);\n"
                       "    float specularFactor = dot(reflectedLightDirection, unitVectorToCamera);\n"
                       "    specularFactor = max(specularFactor,0.0);\n"
                       "    float dampedFactor = pow(specularFactor,shineDamper);\n"
                       "    vec3 totalSpecular = dampedFactor * reflectivity*lightTint;\n"
                       "    fragmentColor = BaseColor*(brightness*lightTint) +vec4(totalSpecular,1.0);\n"
                       "}";*/


    m_fragmentSource = Material::DefaultSourceFragmentShader;

    /*Init Vertex Shader*/
    m_vertexSource = Material::DefaultSourceVertexShader;


    m_fragmentSourceOld = m_fragmentSource;
    m_scene = scene;
    m_b = new QNEBlock(100,50,0,NULL,this);
    m_b->setPos(pos);
    m_scene->addItem(m_b);
    m_b->addPort("Base", 0,NULL,this,-1, QNEPort::NamePort);
    m_b->addPort("Phong", 0,NULL,this,-1, QNEPort::TypePort);
    port1 = m_b->addInputPort("Base Color",NULL,this,0);
    port2 = m_b->addInputPort("normal",NULL,this,1);
    port3 = m_b->addInputPort("roughNess",NULL,this,2);
    port4 = m_b->addInputPort("metalNess",NULL,this,3);
    port5 = m_b->addInputPort("occlusion ambiante",NULL,this,4);

    connect(port1,SIGNAL(haveBeenConnected(QNEPort*,QNEPort*)),this,SLOT(oneInputPortHaveBeenConnect(QNEPort*,QNEPort*)));
    connect(port1,SIGNAL(haveBeenDisconnected(QNEPort*,QNEPort*)),this,SLOT(onePortHaveBeenDisconnect(QNEPort*,QNEPort*)));
    connect(port2,SIGNAL(haveBeenConnected(QNEPort*,QNEPort*)),this,SLOT(oneInputPortHaveBeenConnect(QNEPort*,QNEPort*)));
    connect(port2,SIGNAL(haveBeenDisconnected(QNEPort*,QNEPort*)),this,SLOT(onePortHaveBeenDisconnect(QNEPort*,QNEPort*)));
    connect(port3,SIGNAL(haveBeenConnected(QNEPort*,QNEPort*)),this,SLOT(oneInputPortHaveBeenConnect(QNEPort*,QNEPort*)));
    connect(port3,SIGNAL(haveBeenDisconnected(QNEPort*,QNEPort*)),this,SLOT(onePortHaveBeenDisconnect(QNEPort*,QNEPort*)));
    connect(port4,SIGNAL(haveBeenConnected(QNEPort*,QNEPort*)),this,SLOT(oneInputPortHaveBeenConnect(QNEPort*,QNEPort*)));
    connect(port4,SIGNAL(haveBeenDisconnected(QNEPort*,QNEPort*)),this,SLOT(onePortHaveBeenDisconnect(QNEPort*,QNEPort*)));
    connect(port5,SIGNAL(haveBeenConnected(QNEPort*,QNEPort*)),this,SLOT(oneInputPortHaveBeenConnect(QNEPort*,QNEPort*)));
    connect(port5,SIGNAL(haveBeenDisconnected(QNEPort*,QNEPort*)),this,SLOT(onePortHaveBeenDisconnect(QNEPort*,QNEPort*)));

}

NodeBase::~NodeBase()
{

    /*Delete Parents*/
    foreach(NodeMaterial* node,m_parent)
        SAFE_DELETE(node);


}

QString NodeBase::getFragmentSource()
{
    return m_fragmentSource;
}

QString NodeBase::getVertexSource()
{
    return m_vertexSource;
}

std::vector<QString> NodeBase::getTexturesPath()
{
    return m_texturesPath;
}

void NodeBase::save(QDataStream &ds)
{

    ds << m_fragmentSource;
    ds << m_fragmentSourceOld;
    ds << m_vertexSource;

    ds << m_buildShaderString;
    ds << m_buildShaderUniformString;
    ds << m_texturesPath.size();
    foreach(QString path,m_texturesPath){
        ds << FolderGestion::removeProjectPath(FolderGestion::checkoutReferences(path));
    }


}

void NodeBase::load(QDataStream &ds)
{
    ds >> m_fragmentSource;
    ds >> m_fragmentSourceOld;
    ds >> m_vertexSource;

    ds >> m_buildShaderString;
    ds >> m_buildShaderUniformString;
    size_t size;
    ds >> size;
    for(int i=0;i<size;i++){
        QString path;
        ds >> path;
        m_texturesPath.push_back(FolderGestion::checkoutReferences(path));
    }
}

void NodeBase::addPort(QNEPort *port)
{

    connect(port,SIGNAL(haveBeenConnected(QNEPort*,QNEPort*)),this,SLOT(oneInputPortHaveBeenConnect(QNEPort*,QNEPort*)));
    connect(port,SIGNAL(haveBeenDisconnected(QNEPort*,QNEPort*)),this,SLOT(onePortHaveBeenDisconnect(QNEPort*,QNEPort*)));


}

void NodeBase::emitt()
{
    emit onFragmentSourceChanged(m_fragmentSource);

}

void NodeBase::oneInputPortHaveBeenConnect(QNEPort *in, QNEPort *out)
{

    m_parent.push_back(out->getNode());
    m_parentIn.push_back(in->number());
    buildShader();
}

void NodeBase::onePortHaveBeenDisconnect(QNEPort *in, QNEPort *out)
{
    std::vector<NodeMaterial*>::iterator it;
    std::vector<int>::iterator it2;
    if(in->getNodeBase()==NULL){
        it = find (m_parent.begin(), m_parent.end(), in->getNode());
        it2 = find (m_parentIn.begin(), m_parentIn.end(), in->number());
        if (it != m_parent.end()){
            m_parent.erase(it);
            m_parentIn.erase(it2);
        }

    }else{
        it = find (m_parent.begin(), m_parent.end(), out->getNode());
        it2 = find (m_parentIn.begin(), m_parentIn.end(), in->number());
        if (it != m_parent.end()){
            m_parent.erase(it);
            m_parentIn.erase(it2);
        }

    }
    buildShader();

}

void NodeBase::buildRecursive(NodeMaterial* node,QStringList* listUsedVar){
    if(node){
        foreach (NodeMaterial *child, node->getParents()) {

            if(child->getParents().size()>0){
                buildRecursive(child,listUsedVar);
            }
            if(!listUsedVar->contains(child->getVarName())){
                listUsedVar->push_back(child->getVarName());
                child->setDefaultValue();
                if(child->haveImage()){
                    int i = child->getBinding()-3;
                    if(i<m_texturesPath.size())
                        m_texturesPath.at(i) = child->getImagePath();
                    else{
                        for(int j=m_texturesPath.size();j<=i;j++)
                            m_texturesPath.push_back("");
                        m_texturesPath.at(i) = child->getImagePath();
                    }
                }
                if(child->isUniform())
                    m_buildShaderUniformString += child->getFragmentSource();
                else
                    m_buildShaderString += child->getFragmentSource();
            }
        }
    }
}

void NodeBase::buildShader()
{

    QStringList* listUsedVar = new QStringList();
    m_buildShaderString = "";
    m_buildShaderUniformString = "";
    for(int i=0;i<m_parent.size();i++){
        buildRecursive(m_parent.at(i),listUsedVar);
        if(!listUsedVar->contains(m_parent.at(i)->getVarName())){
            listUsedVar->push_back(m_parent.at(i)->getVarName());
            m_parent.at(i)->setDefaultValue();
            if(m_parent.at(i)->isUniform())
                m_buildShaderUniformString += m_parent.at(i)->getFragmentSource();
            else
                m_buildShaderString += m_parent.at(i)->getFragmentSource();
        }

        if(m_parent.at(i)->haveImage()){
            int i = m_parent.at(i)->getBinding()-3;
            if(i<m_texturesPath.size())
                m_texturesPath.at(i) = m_parent.at(i)->getImagePath();
            else{
                for(int j=m_texturesPath.size();j<=i;j++)
                    m_texturesPath.push_back("");
                m_texturesPath.at(i) = m_parent.at(i)->getImagePath();
            }
        }
        if(m_parentIn.at(i) == 0){
            if( m_parent.at(i)->getTypeVar()==Float)
                m_buildShaderString += "    BaseColor = vec4(" + m_parent.at(i)->getVarName() + ");\n";
            else if( m_parent.at(i)->getTypeVar()==vec2)
            m_buildShaderString += "    BaseColor = vec4(" + m_parent.at(i)->getVarName() + ",0.0,1.0);\n";
            else if( m_parent.at(i)->getTypeVar()==vec3)
                m_buildShaderString += "    BaseColor = vec4(" + m_parent.at(i)->getVarName() + ",1.0);\n";
            else if(m_parent.at(i)->getTypeVar()==vec4)
                m_buildShaderString += "    BaseColor = " + m_parent.at(i)->getVarName() + ";\n";
        }
        if(m_parentIn.at(i)==1){
            m_buildShaderString += "    normalMapped = true;\n";
            if( m_parent.at(i)->getTypeVar()==Float)
                m_buildShaderString += "    BaseColor = 2.0 * vec4(" + m_parent.at(i)->getVarName() + ")- 1.0;\n";
            else if( m_parent.at(i)->getTypeVar()==vec2)
            m_buildShaderString += "    BaseColor = 2.0 * vec4(" + m_parent.at(i)->getVarName() + ",0.0,1.0)- 1.0;\n";
            else if( m_parent.at(i)->getTypeVar()==vec3)
                m_buildShaderString += "    normalValue = 2.0 * vec4(" + m_parent.at(i)->getVarName() + ",1.0)- 1.0;\n";
            else if(m_parent.at(i)->getTypeVar()==vec4)
                m_buildShaderString += "    normalValue  = 2.0 * " + m_parent.at(i)->getVarName() + " - 1.0;\n";
        }
        if(m_parentIn.at(i)==2){
            if( m_parent.at(i)->getTypeVar()==Float)
                m_buildShaderString += "    roughness = " + m_parent.at(i)->getVarName() + ";\n";
        }
        if(m_parentIn.at(i)==3){
            if( m_parent.at(i)->getTypeVar()==Float)
                m_buildShaderString += "    metallic = " + m_parent.at(i)->getVarName() + ";\n";
        }
        if(m_parentIn.at(i)==4){
            if( m_parent.at(i)->getTypeVar()==Float)
                m_buildShaderString += "    ao = " + m_parent.at(i)->getVarName() + ";\n";
        }
    }
    m_fragmentSource = m_fragmentSourceOld;
    int posStr = m_fragmentSourceOld.toStdString().find("//#toReplaceUniform\n");



    if(m_buildShaderUniformString.size()>0)
        m_fragmentSource.replace(posStr,QString("//#toReplaceUniform\n").length(),m_buildShaderUniformString);

    int posStr2 = m_fragmentSource.toStdString().find("//#toReplace\n");


    if(m_buildShaderString.size()>0)
        m_fragmentSource.replace(posStr2,QString("//#toReplace\n").length(),m_buildShaderString);

    delete listUsedVar;
    emit onFragmentSourceChanged(m_fragmentSource);

}
