#ifndef SUBDIVIZIONS_H
#define SUBDIVIZIONS_H

#include <QUuid>



class Subdivizions
{
public:
    Subdivizions(QUuid orgKey);
    QUuid key() const;
    void setKey(const QUuid &key);

    QString name() const;
    void setName(const QString &name);

    QUuid orgKey() const;
    void setOrgKey(const QUuid &orgKey);

private:
    QUuid key_;
    QString name_;
    QUuid orgKey_;
};

#endif // SUBDIVIZIONS_H
