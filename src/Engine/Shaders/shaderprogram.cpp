#include "shaderprogram.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QCoreApplication>
#include <iostream>

ShaderProgram::ShaderProgram()
{

}

void ShaderProgram::init(QString vertexStr)
{

    programID = glCreateProgram();

    vertexID = loadShader(vertexStr,GL_VERTEX_SHADER);


    glAttachShader(programID, vertexID);
    bindAttributes();
    glLinkProgram(programID);
    glValidateProgram(programID);
    getAllUniformLocations();
}

void ShaderProgram::init(QString vertexStr, QString fragmentStr)
{

    programID = glCreateProgram();

    vertexID = loadShader(vertexStr,GL_VERTEX_SHADER);
    fragmentID = loadShader(fragmentStr,GL_FRAGMENT_SHADER);

    glAttachShader(programID, vertexID);
    glAttachShader(programID, fragmentID);
    bindAttributes();
    glLinkProgram(programID);
    glValidateProgram(programID);
    getAllUniformLocations();
}

void ShaderProgram::init(QString vertexStr, QString geoStr, QString fragmentStr)
{

    programID = glCreateProgram();

    vertexID = loadShader(vertexStr,GL_VERTEX_SHADER);
    fragmentID = loadShader(fragmentStr,GL_FRAGMENT_SHADER);
    geoID = loadShader(geoStr,GL_GEOMETRY_SHADER);

    glAttachShader(programID, vertexID);
    glAttachShader(programID, fragmentID);
    glAttachShader(programID, geoID);
    bindAttributes();
    glLinkProgram(programID);
    glValidateProgram(programID);
    glValidateProgram(geoID);
    getAllUniformLocations();
}

void ShaderProgram::initWithSource(QString vertexStr, QString fragmentStr)
{
    programID = glCreateProgram();

    vertexID = loadShaderWithSource(vertexStr,GL_VERTEX_SHADER);
    fragmentID = loadShaderWithSource(fragmentStr,GL_FRAGMENT_SHADER);

    glAttachShader(programID, vertexID);
    glAttachShader(programID, fragmentID);
    bindAttributes();
    glLinkProgram(programID);
    glValidateProgram(programID);
    getAllUniformLocations();
}

void ShaderProgram::getAllUniformLocations(){}
void ShaderProgram::bindAttributes(){}

void ShaderProgram::bindAttribute(int attribute, const char *variableName){
    glBindAttribLocation(programID, attribute, variableName);
}

void ShaderProgram::start(){
    glUseProgram(programID);
}

void ShaderProgram::stop(){
    glUseProgram(0);
}

void ShaderProgram::cleanUp(){
    stop();
    glDetachShader(programID, vertexID);
    glDetachShader(programID, fragmentID);
    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);
    glDeleteShader(geoID);
    glDeleteProgram(programID);
}

int ShaderProgram::loadShader(QString fileName,int type){

    std::string ShaderCode;


    QString QfileName = ":/Shaders/"+fileName;
    QFile fichier(QfileName);

    fichier.open(QIODevice::ReadOnly);
    QTextStream flux(&fichier);

    if(!fichier.isOpen()){
        qDebug() << "Cannot open " << QfileName;
        return 0;
    }

    QString ligne;
    while(! flux.atEnd())
    {

        ligne = flux.readLine();
        ShaderCode += "\n" + ligne.toStdString();
    }


    const GLchar * SourcePointer = (const GLchar *)ShaderCode.c_str();

    int shaderID = glCreateShader(type);
    glShaderSource(shaderID, 1, &SourcePointer, NULL);

    glCompileShader(shaderID);
    GLint isCompiled = 0;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);


        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(shaderID, maxLength, &maxLength, &infoLog[0]);

        std::string bugReport;
        for(int i=0;i<infoLog.size();i++)
            bugReport.push_back(infoLog[i]);


        glDeleteShader(shaderID);
        std::cerr << "Cannot compile shader " << fileName.toStdString() << " " << bugReport << std::endl;


        return 0;
    }

    return shaderID;
}

int ShaderProgram::loadShaderWithSource(QString source, int type)
{

    QString ShaderCode;


    QString QfileName = ":/Shaders/functions.glsl";
    QFile fichier(QfileName);

    fichier.open(QIODevice::ReadOnly);
    QTextStream flux(&fichier);

    if(!fichier.isOpen()){
        qDebug() << "Cannot open " << QfileName;
        return 0;
    }

    QString ligne;
    while(! flux.atEnd())
    {
        ligne = flux.readLine();
        ShaderCode += ligne + "\n" ;
    }

    int posStr = source.toStdString().find("//#toReplaceFunctions\n");



    if(posStr != std::string::npos)
        source.replace(posStr,QString("//#toReplaceFunctions\n").length(),ShaderCode);

    GLchar * SourcePointer = new char[source.size() + 1];
    strcpy(SourcePointer, source.toStdString().c_str());
    //const GLchar * SourcePointer = (const GLchar *)source.toA;

    int shaderID = glCreateShader(type);
    glShaderSource(shaderID, 1, &SourcePointer, NULL);

    glCompileShader(shaderID);
    GLint isCompiled = 0;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);


        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(shaderID, maxLength, &maxLength, &infoLog[0]);

        std::string bugReport;
        for(int i=0;i<infoLog.size();i++)
            bugReport.push_back(infoLog[i]);


        glDeleteShader(shaderID);
        std::cerr << "Cannot compile shader " << bugReport << std::endl;
        return 0;
    }
    delete SourcePointer;
    return shaderID;
}

int ShaderProgram::getUniformLocation(QString uniformName){
    return glGetUniformLocation(programID,uniformName.toStdString().c_str());
}

void ShaderProgram::loadBoolean(int location, bool value){
    float toLoad = 0;
    if(value)
        toLoad = 1;
    glUniform1f(location, toLoad);
}

void ShaderProgram::loadMatrix(int location,glm::mat4  matrix ){
    glUniformMatrix4fv(location,1, false,  (float*)&matrix[0]);
}

void ShaderProgram::loadInt(int location, int value){
    glUniform1i(location, value);
}

void ShaderProgram::loadFloat(int location, float value){
    glUniform1f(location, value);
}

void ShaderProgram::loadVector(int location, glm::vec3 vector){
    glUniform3f(location, vector.x, vector.y, vector.z);
}
