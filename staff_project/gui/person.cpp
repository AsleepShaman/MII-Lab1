#include "person.h"

Person::Person(QUuid orgKey, QUuid subKey)
{
    key_ = QUuid();
    tabID_ = 0;
    surname_ = QString();
    name_ = QString();
    patronomic_ = QString();
    subdivizionKey_ = subKey;
    orgKey_ = orgKey;
    sex_ = 0;
    empDate_ = QDate::currentDate();
    seniority_ = NULL;
//    holiday_ = NULL;
}
/**
 * @brief Метод toString
 * @return Собирает в строку все данные о сотруднике
 */
QString Person::toString() const
{
    return QString::fromUtf8("Табельный номер: %1, ФИО: %2 %3 %4, Пол: %5, Отдел: %6, Дата: %7, Организация: %8")
            .arg(tabID_).arg(surname_).arg(name_).arg(patronomic_)
            .arg(sexTxt()).arg(subdivizionKey_.toString())
            .arg(empDate_.toString("dd.MM.yyyy")).arg(orgKey_.toString());
}

/**
 * @brief Метод FIO
 * @return Конвертирует ФИО в формат Фамилия И. О.
 */
QString Person::FIO() const
{
    return QString::fromUtf8("%1 %2. %3.").arg(surname_).arg(name_.left(1)).arg(patronomic_.left(1));
}

/**
 * @brief Метод sexTxt
 * @return Конвертирует данные о поле сотрудника в строку "м" или "ж"
 */
QString Person::sexTxt() const
{
    QString sexTxt;
    switch (sex_) {
    case MALE:
        sexTxt = QString::fromUtf8("м");
        break;
    case FEMALE:
        sexTxt = QString::fromUtf8("ж");
        break;
    default:
        break;
    }
    return sexTxt;
}

QUuid Person::key() const
{
    return key_;
}
/**
 * @brief Person::setKey
 * @param key
 */
void Person::setKey(const QUuid &key)
{
    key_ = key;
}

uint Person::tabID() const
{
    return tabID_;
}

void Person::setTabID(const uint &tabID)
{
    tabID_ = tabID;
}

QString Person::surname() const
{
    return surname_;
}

void Person::setSurname(const QString &surname)
{
    surname_ = surname;
}

QString Person::name() const
{
    return name_;
}

void Person::setName(const QString &name)
{
    name_ = name;
}

QString Person::patronomic() const
{
    return patronomic_;
}

void Person::setPatronomic(const QString &patronomic)
{
    patronomic_ = patronomic;
}

QUuid Person::subdivizionKey() const
{
    return subdivizionKey_;
}

void Person::setSubdivizionKey(const QUuid &subdivizionkey)
{
    subdivizionKey_ = subdivizionkey;
}

uint Person::sex() const
{
    return sex_;
}

void Person::setSex(const uint &sex)
{
    sex_ = sex;
}

QDate Person::empDate() const
{
    return empDate_;
}

void Person::setEmpDate(const QDate &empDate)
{
    empDate_ = empDate;
}

void Person::setSeniority(qint64 seniority)
{
    if (!seniority_) seniority_ = new qint64(0);
    *seniority_ = seniority;
}

/**
 * @brief Метод seniorityString
 * @return Конвертирует стаж из дней в вид Y лет, M месяцев, D дней
 */
QString Person::seniorityString() const
{
    QString rez;
    qint64 days;
    qint64 mounth;
    qint64 years;
    if (!seniority_) return QString("-");
    rez = QString::number(*seniority_);
    days = *seniority_;
    years = days/365;
    mounth = (days%365)/30;
    days = (days%365)%30;
    rez = QString::fromUtf8("%1г %2м %3д").arg(years).arg(mounth).arg(days);
    return rez;
}

Holiday *Person::holiday() const
{
    return holiday_.value(key_);
}

void Person::addHoliday(Holiday *holiday)
{
    if (holiday == NULL) return;
    holiday_.insert(key_, holiday);
//            insert(person->key(), person);
}

QUuid Person::orgKey() const
{
    return orgKey_;
}

void Person::setOrgKey(const QUuid &orgKey)
{
    orgKey_ = orgKey;
}

