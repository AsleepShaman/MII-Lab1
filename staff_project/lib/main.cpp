#include "main.h"
#include "holidayevaluation.h"
#include <QDebug>

int evaluate_holiday(QString fileName)
{
    qDebug() << "evaluate_holiday";
    HolidayEvaluation holidayEvaluation(fileName);
    if (!holidayEvaluation.exec()) {
        return 1;
    }
    return 0;
}
