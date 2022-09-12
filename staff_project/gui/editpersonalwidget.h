#ifndef EDITPERSONALWIDGET_H
#define EDITPERSONALWIDGET_H

#include <QWidget>
#include "person.h"

namespace Ui {
class EditPersonalWidget;
}

class EditPersonalWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EditPersonalWidget(Person* person, QUuid orgKey, QUuid subKey, QWidget *parent = 0);
    ~EditPersonalWidget();
signals:
    void person_edited(Person*, bool);
private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_yes_clicked();

private:
    Ui::EditPersonalWidget *ui;
        Person* person_;
        QUuid orgKey_;
        QUuid subKey_;
};

#endif // EDITPERSONALWIDGET_H
