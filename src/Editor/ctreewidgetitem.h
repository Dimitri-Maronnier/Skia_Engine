
#ifndef CTREEWIDGETITEM_H
#define CTREEWIDGETITEM_H
#include <QTreeWidgetItem>

enum TypeWidgetItem{directory = 0, file = 1};

class CTreeWidgetItem:public QTreeWidgetItem
{
public:

    CTreeWidgetItem();
    bool isDirectory();
    void setType(TypeWidgetItem type);
    std::string getOldName();
    QString getExt();
    void setOldName(std::string oldName);
    void setExt(QString ext);
private:
    TypeWidgetItem m_type;
    std::string m_oldName;
    QString m_ext;
};

#endif // CTREEWIDGETITEM_H
