#include "orgstruct.h"
#include <QtXml>
#include <QtXmlPatterns/QXmlQuery>
#include <QDesktopServices>
#include <QUrl>
#include <QtSql>


OrgStruct::OrgStruct()
{

}

QString OrgStruct::toString() const
{
    QStringList liststr;
    foreach (const Organization* org, organizations_) {
        liststr << org->toString();
    }
    return liststr.join('\n');
}

QMap<QUuid, Person *> OrgStruct::staff() const
{
    return persons_;
}

QMap<QUuid, Organization *> OrgStruct::organizations() const
{
    return organizations_;
}

void OrgStruct::addPerson(Person *person)
{
    if (person == NULL) return;
    persons_.insert(person->key(), person);
}

Person *OrgStruct::takePerson(QUuid key)
{
    return persons_.take(key);
}

Person *OrgStruct::getPerson(QUuid key)
{
    return persons_.value(key);
}

void OrgStruct::addSubdiv(Subdivizions *sub)
{
    if (sub == NULL) return;
    subs_.insert(sub->key(), sub);
}

Subdivizions *OrgStruct::takeSubdiv(QUuid key)
{
    return subs_.take(key);
}

Subdivizions *OrgStruct::getSubdiv(QUuid key)
{
    return subs_.value(key);
}

QMap<QUuid, Subdivizions *> OrgStruct::subs() const
{
    return subs_;
}

QString OrgStruct::toString_per() const
{
    QStringList liststr;
    foreach (const Person* per, persons_) {
        liststr << per->toString();
    }
    return liststr.join('\n');
}

void OrgStruct::addOrganization(Organization *org)
{
    if (org == NULL) return;
    organizations_.insert(org->key(), org);
}

Organization *OrgStruct::getOrganization(QUuid key)
{
    return organizations_[key];
}

Organization *OrgStruct::takeOrganization(QUuid key)
{
        return organizations_.take(key);
}

QString OrgStruct::loadFromXML(QString fileName)
{
    QString msg;
    QDomDocument domDoc;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        msg = QString::fromUtf8("Ошибка при открытии файла %1").arg(fileName);
        qWarning() << msg;
        return msg;
    }

    if(!domDoc.setContent(&file))
    {
        msg = QString::fromUtf8("Ошибка при чтении файла %1").arg(fileName);
        qWarning() << msg;
        file.close();
        return msg;
    }
    file.close();
//    qDebug().noquote() << domDoc.toString(2);

    QDomElement e_org = domDoc.firstChildElement().firstChildElement("Organization");

    while(!e_org.isNull())
    {
        //qDebug() <<e_org.tagName() << e_org.attribute("Name") <<e_org.attribute("UUID");

        Organization* org = new Organization();
        org->setKey(e_org.attribute("UUID"));
        org->setName(e_org.attribute("Name"));

        QDomElement e_subs = e_org.firstChildElement("Subdivizion");
        while(!e_subs.isNull()){

            Subdivizions* sub = new Subdivizions(org->key());
            sub->setKey(e_subs.attribute("UUID"));
            sub->setName(e_subs.attribute("Name"));


            QDomElement e_person = e_subs.firstChildElement("Person");
            while(!e_person.isNull())
            {
    //            qDebug() << "load from xml:" <<e_person.tagName() <<e_person.attribute("UUID");

                Person* person = new Person(org->key(), sub->key());
                person->setKey(e_person.attribute("UUID"));
                person->setTabID(e_person.firstChildElement("Tab_ID").text().toUInt());
                person->setName(e_person.firstChildElement("Name").text());
                person->setSurname(e_person.firstChildElement("Surname").text());
                person->setPatronomic(e_person.firstChildElement("Patronymic").text());
//                person->setSubdivizionKey(sub->key());
//                qDebug() << person->subdivizionKey() << "----" << sub->key();
                QString sexTxt = e_person.firstChildElement("Sex").text();
                uint sex = (sexTxt==QString::fromUtf8("м"))? Person::MALE : Person::FEMALE;
                person->setSex(sex);
                QDate date = QDate::fromString(e_person.firstChildElement("Emp_date").text(), "yyyy-MM-dd");
                person->setEmpDate(date);
                qint64 seniority = date.daysTo(QDate::currentDate());
                person->setSeniority(seniority);
//                person->setOrgKey(org->key());

    //            qDebug() << person->toString() << person->key() << person->orgKey();

                QDomElement e_hol = e_person.firstChildElement("Holidays");
    //            qDebug() << e_person.firstChildElement("Holidays").isNull();

                if(!e_person.firstChildElement("Holidays").isNull()){
                    Holiday* hols = new Holiday();
                    date = QDate::fromString(e_hol.firstChildElement("Begin_Date").text(), "dd.MM.yyyy");
    //                qDebug() << "Begin_Date" << e_hol.firstChildElement("Begin_Date").text();
                    hols->setBegin(date);
                    hols->setDuration(e_hol.firstChildElement("Duration").text().toUInt());
    //                qDebug() << "Duration" << e_hol.firstChildElement("Duration").text().toUInt();
                    date = QDate::fromString(e_hol.firstChildElement("End_Date").text(), "dd.MM.yyyy");
    //                qDebug() << "End_Date" << date;
                    hols->setEnd(new QDate(date));

                    person->addHoliday(hols);
                }
                persons_.insert(person->key(), person);
    //            org->addPerson(person);
    //            qDebug() << toString_per();
                e_person = e_person.nextSiblingElement("Person");
            }
            subs_.insert(sub->key(), sub);
            e_subs = e_subs.nextSiblingElement("Subdivizion");
        }

        //qDebug().noquote()<< "Organization" << org->toString();
        organizations_.insert(org->key(), org);
        e_org = e_org.nextSiblingElement("Organization");
    }
    qDebug().noquote() << "Orglist" << toString();
    return msg;
}

bool OrgStruct::toXML(QString fileName) const
{
//    qDebug() << "to XML" << fileName;
    QDomDocument doc;
    doc.appendChild(doc.createProcessingInstruction(QString("xml"), QString("version = \"1.0\" encoding = \"UTF-8\"")));
    QDomElement staff = doc.createElement("Staff");
    doc.appendChild(staff);
    foreach (const Organization* org, organizations_)
    {
        QDomElement org_xml = doc.createElement("Organization");
            QDomAttr org_name = doc.createAttribute("Name");
            org_name.setValue(org->name());
            QDomAttr org_UUID = doc.createAttribute("UUID");
            org_UUID.setValue(org->key().toString());
        org_xml.setAttributeNode(org_UUID);
        org_xml.setAttributeNode(org_name);
        staff.appendChild(org_xml);

        foreach (const Subdivizions* sub, subs_) {
            if (sub->orgKey()==org->key()){
                QDomElement sub_xml = doc.createElement("Subdivizion");
                    QDomAttr sub_name = doc.createAttribute("Name");
                    sub_name.setValue(sub->name());
                    QDomAttr sub_UUID = doc.createAttribute("UUID");
                    sub_UUID.setValue(sub->key().toString());
                sub_xml.setAttributeNode(sub_UUID);
                sub_xml.setAttributeNode(sub_name);
                org_xml.appendChild(sub_xml);

                foreach (const Person* per, persons_)
                {
                    if (per->orgKey() == org->key() && per->subdivizionKey() == sub->key()){
                        QDomElement per_xml = doc.createElement("Person");
                        QDomAttr per_uuid = doc.createAttribute("UUID");
                        per_uuid.setNodeValue(per->key().toString());
                    per_xml.setAttributeNode(per_uuid);
        //            qDebug() << per->key();

                    QDomElement per_tabID = doc.createElement("Tab_ID");
                        QDomText t = doc.createTextNode(QString::number(per->tabID()));
                        per_tabID.appendChild(t);
                    per_xml.appendChild(per_tabID);

                    QDomElement per_surname = doc.createElement("Surname");
                        t = doc.createTextNode(per->surname());
                        per_surname.appendChild(t);
                    per_xml.appendChild(per_surname);

                    QDomElement per_name = doc.createElement("Name");
                        t = doc.createTextNode(per->name());
                        per_name.appendChild(t);
                    per_xml.appendChild(per_name);

                    QDomElement per_patronymic = doc.createElement("Patronymic");
                        t = doc.createTextNode(per->patronomic());
                        per_patronymic.appendChild(t);
                    per_xml.appendChild(per_patronymic);



                    QDomElement per_sex = doc.createElement("Sex");
                        t = doc.createTextNode(per->sexTxt());
                        per_sex.appendChild(t);
                    per_xml.appendChild(per_sex);

                    QDomElement per_empDate = doc.createElement("Emp_date");
                        t = doc.createTextNode(per->empDate().toString("yyyy-MM-dd"));
                        per_empDate.appendChild(t);
                    per_xml.appendChild(per_empDate);

                    if (per->holiday()){

                        QDomElement holidays = doc.createElement("Holidays");
                        per_xml.appendChild(holidays);

                            QDomElement beginDate = doc.createElement("Begin_Date");
                            QDomText t = doc.createTextNode(per->holiday()->begin().toString("dd.MM.yyyy"));
                            beginDate.appendChild(t);
                            holidays.appendChild(beginDate);

                            QDomElement duration = doc.createElement("Duration");
                            t = doc.createTextNode(QString::number(per->holiday()->duration()));
                            duration.appendChild(t);
                            holidays.appendChild(duration);

                            if (per->holiday()->end()){
                                QDomElement end = doc.createElement("End_Date");
                                t = doc.createTextNode(per->holiday()->endString());

                                end.appendChild(t);
                                holidays.appendChild(end);
                            }
                        }
                    //QDomElement per_sex = doc.createElement("Sex");
                    sub_xml.appendChild(per_xml);
                    }
                }
            }

        }
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qWarning() << QString::fromUtf8("Не удалось открыть файл %1 для записи").arg(fileName);
        return false;
    }
    file.write(doc.toByteArray(3));
    file.close();
    qDebug().noquote() << doc.toString(3);
    return true;

}

bool OrgStruct::toXMLReport(QString fileName) const
{
    QDomDocument doc;
    doc.appendChild(doc.createProcessingInstruction(QString("xml"), QString("version = \"1.0\" encoding = \"UTF-8\"")));
    doc.appendChild(doc.createProcessingInstruction(QString("xml-stylesheet"), QString("type = \"text/xsl\" href = \"organizations-style.xslt\"")));
    QDomElement staff = doc.createElement("Staff");
    doc.appendChild(staff);
    foreach (const Organization* org, organizations_)
    {
        QDomElement org_xml = doc.createElement("Organization");
        QDomAttr org_name = doc.createAttribute("Name");
        org_name.setValue(org->name());
        org_xml.setAttributeNode(org_name);
        staff.appendChild(org_xml);

        foreach (const Person* per, persons_)
        {
            if(per->orgKey() == org->key()){
                QDomElement per_xml = doc.createElement("Person");
                QDomElement per_tabID = doc.createElement("Tab_ID");
                QDomText t = doc.createTextNode(QString::number(per->tabID()));
                per_tabID.appendChild(t);
                per_xml.appendChild(per_tabID);

                QDomElement per_surname = doc.createElement("Surname");
                t = doc.createTextNode(per->surname());
                per_surname.appendChild(t);
                per_xml.appendChild(per_surname);

                QDomElement per_name = doc.createElement("Name");
                t = doc.createTextNode(per->name());
                per_name.appendChild(t);
                per_xml.appendChild(per_name);

                QDomElement per_patronymic = doc.createElement("Patronymic");
                t = doc.createTextNode(per->patronomic());
                per_patronymic.appendChild(t);
                per_xml.appendChild(per_patronymic);

                QDomElement per_subdivizion = doc.createElement("Subdivizion");
                t = doc.createTextNode(subs_.value(per->subdivizionKey())->name());
                per_subdivizion.appendChild(t);
                per_xml.appendChild(per_subdivizion);

                QDomElement per_sex = doc.createElement("Sex");
                t = doc.createTextNode(per->sexTxt());
                per_sex.appendChild(t);
                per_xml.appendChild(per_sex);

                QDomElement per_empDate = doc.createElement("Emp_date");
                t = doc.createTextNode(per->empDate().toString("yyyy-MM-dd"));
                per_empDate.appendChild(t);
                per_xml.appendChild(per_empDate);

                if (per->holiday()){
                    QDomElement holidays = doc.createElement("Holidays");
                    per_xml.appendChild(holidays);

                    QDomElement beginDate = doc.createElement("Begin_Date");
                    QDomText t = doc.createTextNode(per->holiday()->begin().toString("dd.MM.yyyy"));
                    beginDate.appendChild(t);
                    holidays.appendChild(beginDate);

                    if (per->holiday()->end()){
                        QDomElement end = doc.createElement("End_Date");
                        t = doc.createTextNode(per->holiday()->endString());
                        end.appendChild(t);
                        holidays.appendChild(end);
                    }
                }
                org_xml.appendChild(per_xml);
            }

        }
    }

    QFile file(fileName); // можно сделать временным.
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qWarning() << QString::fromUtf8("Не удалось открыть файл %1 для записи").arg(fileName);
        return false;
    }
    file.write(doc.toByteArray(3));
    file.close();

    QString html("");
        QXmlQuery query(QXmlQuery::XSLT20);
        query.setFocus(QUrl(fileName));
        query.setQuery(QUrl("/work/practica/staff/bin/organizations-style.xslt"));
        query.evaluateTo(&html);

        QFile htmlFile("/work/practica/staff/bin/raport.rtf");
        if (!htmlFile.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            qWarning() << QString::fromUtf8("Не удалось открыть файл %1 для записи").arg(htmlFile.fileName());
            return false;
        }
        htmlFile.write(html.toUtf8());
        htmlFile.close();
         QDesktopServices::openUrl(QUrl(htmlFile.fileName()));
    qDebug().noquote() << html;
    return true;
}

bool OrgStruct::toDB() const
{
    if (!createConnection()) return false;

    QSqlQuery query;

    QString org_insert = "INSERT INTO organization(key, name) "
                         "VALUES('%1', '%2')";

    QString sub_insert = "INSERT INTO subdivizion(key, name, org_key) "
                         "VALUES('%1', '%2', '%3')";

    QString per_insert = "INSERT INTO personal(key, name, surname, patronomic, sex, emp_date, seniority, org_key, sub_key, tab_id) "
                         "VALUES('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10')";
    QString str;
    query.exec("DELETE FROM organization *");
    foreach (const Organization* org, organizations_) {
        str = org_insert.arg(org->key().toString()).arg(org->name());
        if(!query.exec(str)){ qDebug() << "Organization:" << query.lastError(); return false;}
        foreach (const Subdivizions* sub, subs_) {
            if(sub->orgKey()==org->key()){
                str = sub_insert.arg(sub->key().toString()).arg(sub->name()).arg(sub->orgKey().toString());
                if(!query.exec(str)){qDebug() << "Subdivizion:" << query.lastError();return false;}
                foreach (const Person* per, persons_) {
                    if(per->orgKey()==org->key() && per->subdivizionKey()==sub->key()){
                        str = per_insert.arg(per->key().toString()).arg(per->name()).arg(per->surname())
                                .arg(per->patronomic()).arg(per->sexTxt()).arg(per->empDate().toString("yyyy-MM-dd"))
                                .arg(per->seniorityString()).arg(per->orgKey().toString())
                                .arg(per->subdivizionKey().toString()).arg(per->tabID());
                        if(!query.exec(str)){qDebug() << "Personal:" << query.lastError();return false;}
                    }
                }
            }
        }
    }
    return true;
}

bool OrgStruct::fromDB()
{
    if (!createConnection()) return false;
    QSqlQuery query_org;
    QSqlQuery query_sub;
    QSqlQuery query_per;

    if(!query_org.exec("SELECT * FROM organization;")) {qDebug() << "Organization:" << query_org.lastError();return false;}
    while(query_org.next()){
        Organization* org = new Organization();
        org->setKey(query_org.value(0).toUuid());
        org->setName(query_org.value(1).toString());
        organizations_.insert(org->key(), org);
        if(!query_sub.exec("SELECT * FROM subdivizion;")) {qDebug() << "Subdivizion:" << query_sub.lastError();return false;}
        while(query_sub.next()){
            if(query_sub.value(2)!=org->key()){continue;}
                Subdivizions* sub = new Subdivizions(org->key());
                sub->setKey(query_sub.value(0).toUuid());
                sub->setName(query_sub.value(1).toString());
                subs_.insert(sub->key(), sub);
                if(!query_per.exec("SELECT * FROM personal;")) {qDebug() << "Personal:" << query_org.lastError();return false;}
                while(query_per.next()){
                    if(query_per.value(7)==org->key() && query_per.value(8)==sub->key()){
                        Person* per = new Person(org->key(), sub->key());
                        per->setKey(query_per.value(0).toUuid());
                        per->setTabID(query_per.value(9).toUInt());
                        per->setName(query_per.value(1).toString());
                        per->setSurname(query_per.value(2).toString());
                        per->setPatronomic(query_per.value(3).toString());
                        uint sex = (query_per.value(4).toString()==QString::fromUtf8("м"))? Person::MALE : Person::FEMALE;
                        per->setSex(sex);
                        per->setEmpDate(query_per.value(5).toDate());
                        qint64 seniority = per->empDate().daysTo(QDate::currentDate());
                        per->setSeniority(seniority);
                        persons_.insert(per->key(), per);
                    }
                }
        }
    }
    return true;
}

bool OrgStruct::createConnection() const
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setDatabaseName("Staff");
    db.setUserName("postgres");
    db.setHostName("127.0.0.1");
    if (!db.open()) return false;
    return true;
}


