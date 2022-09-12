#include "subdivizions.h"

Subdivizions::Subdivizions(QUuid orgKey)
{
    orgKey_ = orgKey;
}

QUuid Subdivizions::key() const
{
    return key_;
}

void Subdivizions::setKey(const QUuid &key)
{
    key_ = key;
}

QString Subdivizions::name() const
{
    return name_;
}

void Subdivizions::setName(const QString &name)
{
    name_ = name;
}

QUuid Subdivizions::orgKey() const
{
    return orgKey_;
}

void Subdivizions::setOrgKey(const QUuid &orgKey)
{
    orgKey_ = orgKey;
}
