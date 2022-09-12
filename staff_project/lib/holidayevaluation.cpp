#include "holidayevaluation.h"
#include <QDebug>
#include <QDomDocument>
#include <QFile>


bool HolidayEvaluation::loadFromXML()
{
    QString msg;
    QDomDocument domDoc;
    QFile file(fileName_);
    if (!file.open(QIODevice::ReadOnly))
    {
        msg = QString::fromUtf8("Ошибка при открытии файла %1").arg(fileName_);
        qWarning() << msg;
        return false;
    }

    if(!domDoc.setContent(&file))
    {
        msg = QString::fromUtf8("Ошибка при чтении файла %1").arg(fileName_);
        qWarning() << msg;
        file.close();
        return false;
    }
    file.close();

    qDebug() << fileName_;

        QDomElement e_person = domDoc.firstChildElement().firstChildElement("Person");
        while(!e_person.isNull())
        {
            //qDebug() <<e_person.tagName() <<e_person.attribute("UUID")<< e_person.firstChildElement("Name").text() ;
            Holiday* holiday = new Holiday();
            QUuid key = e_person.attribute("UUID");
            QDate date = QDate::fromString(e_person.firstChildElement("Begin_Date").text(), "dd.MM.yyyy");
            holiday->setBegin(date);
            holiday->setDuration(e_person.firstChildElement("Duration").text().toUInt());

            holidays_.insert(key, holiday);
            e_person = e_person.nextSiblingElement("Person");
//            qDebug() << key << holiday->begin()<< holiday->duration();
        }
//    qDebug() << holidays_ ;

    return true;
}

bool HolidayEvaluation::toXML()
{
    QDomDocument doc;
    doc.appendChild(doc.createProcessingInstruction(QString("xml"), QString("version = \"1.0\" encoding = \"UTF-8\"")));
    QDomElement holidays = doc.createElement("Holidays");
    doc.appendChild(holidays);

    foreach (Holiday* hol, holidays_) {
        QDomElement person = doc.createElement("Person");
            QDomAttr per_uuid = doc.createAttribute("UUID");
            per_uuid.setNodeValue(holidays_.key(hol).toString());
            person.setAttributeNode(per_uuid);
            holidays.appendChild(person);

        QDomElement beginDate = doc.createElement("Begin_Date");
            QDomText t = doc.createTextNode(hol->begin().toString("dd.MM.yyyy"));
            beginDate.appendChild(t);
            person.appendChild(beginDate);

        QDomElement duration = doc.createElement("Duration");
            t = doc.createTextNode(QString::number(hol->duration()));
            duration.appendChild(t);
            person.appendChild(duration);

            QString valueStr = "0";
            QDomElement endDate = doc.createElement("End_Date");
            if(hol->end()) {valueStr = hol->end()->toString("dd.MM.yyyy");}
                t = doc.createTextNode(valueStr);
                endDate.appendChild(t);
                person.appendChild(endDate);
    }

    qDebug().noquote() << doc.toString();

    QFile file(fileName_);
    if (file.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            qDebug() << file.fileName();
            file.write(doc.toByteArray(3));
            file.close();
    } else {
        qDebug() << QString::fromUtf8("Неудачно сохранился");
        return false;
    }

    return true;
}

bool HolidayEvaluation::exec()
{
    if (!loadFromXML()) {return false;}
    QDate endDate;
    foreach (Holiday* holidays, holidays_) {
        endDate = holidays->begin().addDays(holidays->duration());
        if(endDate.isValid()){
            holidays->setEnd(new QDate(endDate));
        }
//        qDebug() << holidays->toString();
    }
    return toXML();

//    return true;
}

QString HolidayEvaluation::toString() const
{
    QStringList liststr;
    foreach (const Holiday* hol, holidays_) {
        liststr << hol->toString();
    }
    return liststr.join('\n');
}

