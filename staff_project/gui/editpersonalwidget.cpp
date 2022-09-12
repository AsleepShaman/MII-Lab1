#include "editpersonalwidget.h"
#include "ui_editpersonalwidget.h"
#include <QDebug>
#include <QUuid>

EditPersonalWidget::EditPersonalWidget(Person* person, QUuid orgKey, QUuid subKey, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditPersonalWidget)
{
    ui->setupUi(this);
    qDebug() << "EditPersonalWidget";
    QStringList Sexlst;
    Sexlst << "м" << "ж";
    ui->comboBox_Sex->addItems(Sexlst);

    orgKey_ = orgKey;
    subKey_ = subKey;
    person_ = person;
    if (person_)
    {
        ui->lineEdit_TabID->setText(QString::number(person_->tabID()));
        ui->lineEdit_Name->setText(person_->name());
        ui->lineEdit_Surname->setText(person_->surname());
        ui->lineEdit_Patronomyc->setText(person_->patronomic());
//        ui->lineEdit_Subdivizion->setText(person_->subdivizion());
        ui->comboBox_Sex->setCurrentIndex(person_->sex());
        ui->dateEdit_empDate->setDate(person_->empDate());        
    }
}

EditPersonalWidget::~EditPersonalWidget()
{
    delete ui;
}

void EditPersonalWidget::on_pushButton_cancel_clicked()
{
    qDebug() << "close";
    close();
}

void EditPersonalWidget::on_pushButton_yes_clicked()
{

    bool isNew = false;
    if (person_ == NULL) { person_ = new Person(orgKey_, subKey_); isNew = true; }

    if (isNew) {
        person_->setKey(QUuid::createUuid());
    }

//    qDebug() << "editform" << orgKey_ << "---" << subKey_;
    person_->setTabID(ui->lineEdit_TabID->text().toUInt());
    person_->setName(ui->lineEdit_Name->text());
    person_->setSurname(ui->lineEdit_Surname->text());
    person_->setPatronomic(ui->lineEdit_Patronomyc->text());
    person_->setSex(ui->comboBox_Sex->currentIndex());
//    qDebug() << "editform" << person_->orgKey() << "---" << person_->subdivizionKey();
//    person_->setSubdivizionKey(ui->lineEdit_Subdivizion->text().toUuid);
    person_->setEmpDate(ui->dateEdit_empDate->date());
    qint64 seniority = person_->empDate().daysTo(QDate::currentDate());
    person_->setSeniority(seniority);


//    qDebug() << person_->toString();
    qDebug() << "до эмита" << person_->toString();
    emit person_edited(person_,isNew);
    qDebug() << "после эмита" << person_->toString();
    close();
}
