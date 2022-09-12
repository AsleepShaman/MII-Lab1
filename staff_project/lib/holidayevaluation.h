#ifndef HOLIDAYEVALUATION_H
#define HOLIDAYEVALUATION_H
#include "holiday.h"
#include <QString>
#include <QMap>
#include <QUuid>
class HolidayEvaluation
{

public:
    HolidayEvaluation(QString fileName){ fileName_ = fileName;}

    bool exec();

    QString toString() const;

private:
    QString fileName_;
    QMap <QUuid, Holiday*> holidays_;

    bool loadFromXML();
    bool toXML();
};

#endif // HOLIDAYEVALUATION_H
