#include "ctreewidgetitem.h"

CTreeWidgetItem::CTreeWidgetItem()
{
    m_type = directory;
    m_oldName = "";
    m_ext = "";
}

bool CTreeWidgetItem::isDirectory(){
    return m_type == directory;
}

void CTreeWidgetItem::setType(TypeWidgetItem type){
    m_type = type;
}

std::string CTreeWidgetItem::getOldName(){
    return m_oldName;
}

QString CTreeWidgetItem::getExt()
{
    return m_ext;
}

void CTreeWidgetItem::setOldName(std::string oldName){
    m_oldName = oldName;
}

void CTreeWidgetItem::setExt(QString ext)
{
    m_ext = ext;
}
