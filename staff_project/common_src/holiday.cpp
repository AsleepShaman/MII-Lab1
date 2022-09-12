#include "holiday.h"

Holiday::Holiday()
{
    end_ = NULL;
}

QDate Holiday::begin() const
{
    return begin_;
}

void Holiday::setBegin(const QDate &begin)
{
    begin_ = begin;
}

uint Holiday::duration() const
{
    return duration_;
}

void Holiday::setDuration(const uint &duration)
{
    duration_ = duration;
}

QDate *Holiday::end() const
{
    return end_;
}

void Holiday::setEnd(QDate *end)
{
    end_ = end;
}

QString Holiday::endString() const
{
    QString rez;
    if (!end_) return QString("-");
    rez = end_->toString("dd.MM.yyyy");
    return rez;
}

QString Holiday::toString() const
{
    QString str = QString::fromUtf8("Начало: %1, Длительность: %2, Конец:%3 ")
            .arg(begin_.toString()).arg(duration_).arg(end_->toString());
    return str;
}
