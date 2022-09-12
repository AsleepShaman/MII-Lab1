#include "organization.h"

Organization::Organization()
{

}
/**
 * @brief Метод toString
 * @return Собирает в строку данные об организации и ее сотрудниках
 */
QString Organization::toString() const
{

    QString str = QString::fromUtf8("UUID: %1, Название: %2")
            .arg(key_.toString()).arg(name_);

    return str;
}





//QMap<QUuid, Person *> Organization::staff() const
//{
//    return staff_;
//}

QUuid Organization::key() const
{
    return key_;
}

void Organization::setKey(const QUuid &key)
{
    key_ = key;
}

QString Organization::name() const
{
    return name_;
}

void Organization::setName(const QString &name)
{
    name_ = name;
}



