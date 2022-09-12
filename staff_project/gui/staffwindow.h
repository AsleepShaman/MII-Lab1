#ifndef STAFFWINDOW_H
#define STAFFWINDOW_H
#include "orgstruct.h"
#include <QListWidget>
#include <QMainWindow>
#include <QMenu>
#include <QTreeWidget>
#include "editpersonalwidget.h"
#include <QTableWidgetItem>

namespace Ui {
class StaffWindow;
}

class StaffWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StaffWindow(QWidget *parent = 0);
    ~StaffWindow();
    enum PersonalColums {TABID, FIO, SEX, DATE, SENIORITY, SUBDIV};
    enum HolidayColums {HC_TABID, HC_FIO, HC_SUBDIV, HC_BEGINDATE, HC_DURATION, HC_ENDDATE};


private slots:
    void on_toolButton_AddPer_clicked();

    void on_toolButton_EditPer_clicked();

    void on_toolButton_DelPer_clicked();

    void on_toolButton_DelOrg_clicked();

    void on_toolButton_AddOrg_clicked();

    void open_file();

    void save_file();

    void report();

    void fill_orgs(QUuid orgKey, QTreeWidgetItem* parent);

    void calc_holidays();

    void current_tab_changed(int idxTab);

    void current_org_changed(QTreeWidgetItem *OrgItem);

    void fill_pers(QUuid orgKey);

    void edit_org(QTreeWidgetItem *itemOrg);

    void set_buttons_enabled(bool enableorg, bool enableper);

    void person_edited(Person* person, bool isNew);

    void help();

    void fill_holiday_table(QUuid orgKey);

    void on_tableWidget_holiday_itemChanged(QTableWidgetItem *item);

    void from_XML_holidays(QString fileName);

    void AddSub();

    void fromDataBase();

    void inDataBase();

private:
    Ui::StaffWindow *ui;
    OrgStruct* orgstruct_;
    EditPersonalWidget* editPersonalWidget_;
    void show_form(Person* person, QUuid orgKey, QUuid subKey);
    QMenu* treeMenu;

};

#endif // STAFFWINDOW_H
