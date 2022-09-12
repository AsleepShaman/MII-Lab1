#ifndef ORGSTRUCT_H
#define ORGSTRUCT_H
#include "organization.h"


class OrgStruct
{
public:
    OrgStruct();
    QString loadFromXML(QString fileName);
    bool toXML(QString fileName) const;
    bool toXMLReport(QString fileName) const;
    bool toDB() const;
    bool fromDB();
    bool createConnection() const;
    QString toString() const;
    QMap<QUuid, Organization *> organizations() const;

    void addPerson(Person *person);
    Person* takePerson (QUuid key);
    Person* getPerson(QUuid key);
    QString toString_per() const;

    QMap<QUuid, Person *> staff() const;

    void addSubdiv(Subdivizions *sub);
    Subdivizions* takeSubdiv (QUuid key);
    Subdivizions* getSubdiv(QUuid key);

    QMap<QUuid, Subdivizions *> subs() const;

    void addOrganization(Organization *org);
    Organization* getOrganization(QUuid key);
    Organization* takeOrganization (QUuid key);

private:
    QMap <QUuid, Organization*> organizations_;
    QMap <QUuid, Person*> persons_;
    QMap <QUuid, Subdivizions*> subs_; //! Список отделов организации
};

#endif // ORGSTRUCT_H
