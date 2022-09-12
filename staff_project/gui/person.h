#ifndef PERSON_H
#define PERSON_H
#include <QUuid>
#include <QDate>
#include <QMap>
#include "holiday.h"
/**
 * @brief Класс сотрудников
 * Содержит поля для описания сорудников организации: табельный номер, ФИО, отдел, дату приема на работу и стаж работы в днях.
 *
 */
class Person
{
public:
    /**
     * @brief The Sex enum
     * Описывает пол сотрудников
     */
    enum Sex{MALE, //!< Мужской
             FEMALE//!< Женский
            };
public:
    Person(QUuid orgKey, QUuid subKey);

    QString toString() const;

    QString FIO() const;

    QString sexTxt() const;

    QUuid key() const;
    void setKey(const QUuid &key);

    uint tabID() const;
    void setTabID(const uint &tabID);

    QString surname() const;
    void setSurname(const QString &surname);

    QString name() const;
    void setName(const QString &name);

    QString patronomic() const;
    void setPatronomic(const QString &patronomic);

    QUuid subdivizionKey() const;
    void setSubdivizionKey(const QUuid &subdivizionkey);

    uint sex() const;
    void setSex(const uint &sex);

    QDate empDate() const;
    void setEmpDate(const QDate &empDate);

    void setSeniority(qint64 seniority);
    QString seniorityString () const;

    Holiday *holiday() const;
    void addHoliday(Holiday *holiday);

    QUuid orgKey() const;
    void setOrgKey(const QUuid &orgKey);

private:
    QUuid key_; //!< Ключ - UUID сотрудника, генерируется автоматически
    uint tabID_; //!< Табельный номер
    QString surname_; //!< Фамилия
    QString name_; //!< Имя
    QString patronomic_; //!< Отчество
    QUuid subdivizionKey_; //!< ключ отдела
    uint sex_; //!< Пол
    QDate empDate_; //!< Дата приема на работу
    qint64* seniority_; //!< Стаж в днях
    QUuid orgKey_; //!ключ организации
    QMap <QUuid, Holiday*> holiday_;
};


#endif // PERSON_H
