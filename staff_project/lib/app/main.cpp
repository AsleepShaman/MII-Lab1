#include <QCoreApplication>
#include <QtDebug>
#include <QDateTime>
#include <QFile>
#include "holidayevaluation.h"

int main(int argc, char *argv[])
{

    if (argc == 2) {
        qDebug() << argv[0] << argv[1];
        QString inputFileName = QString(argv[1]);
        QString outFileName = QString("%1.out.%2.xml").arg(inputFileName).arg(QDateTime::currentDateTime().toString("hhmmssddMMyyyy"));
        qDebug() << QFile::copy(inputFileName, outFileName);

        HolidayEvaluation holidayEvaluation(outFileName);


        if (!holidayEvaluation.exec()) {
            qDebug() << "Error";
        }


        qDebug() << outFileName;
    } else {
        qDebug() << argv[0] << " inputFileName";
    }
}
