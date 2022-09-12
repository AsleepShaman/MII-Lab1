#ifndef HOLIDAY_H
#define HOLIDAY_H
#include <QDate>

class Holiday
{
public:
    Holiday();
    QDate begin() const;
    void setBegin(const QDate &begin);

    uint duration() const;
    void setDuration(const uint &duration);

    QDate *end() const;
    void setEnd(QDate *end);

    QString endString() const;
    QString toString() const;

private:
    QDate begin_;
    uint duration_;
    QDate *end_;
};


#endif // HOLIDAY_H
