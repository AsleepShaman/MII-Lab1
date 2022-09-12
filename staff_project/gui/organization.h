#ifndef ORGANIZATION_H
#define ORGANIZATION_H
#include "person.h"
#include "subdivizions.h"
#include <QUuid>
#include <QMap>

/**
 * @brief Класс организаций
 * содержит данные об организациях и работающих в них сотрудниках
 */
class Organization
{
public:
    Organization();
    QUuid key() const;
    void setKey(const QUuid &key);

    QString name() const;
    void setName(const QString &name);


    QString toString() const;

//    QMap<QUuid, Person *> staff() const;




private:
    QUuid key_; //! Ключ - UUID организации, генерируется автоматически
    QString name_; //! Название организации

};

#endif // ORGANIZATION_H
