#include "staffwindow.h"
#include "ui_staffwindow.h"
#include "spinboxdelegate.h"
#include "dateeditdelegate.h"
#include <QDebug>
#include <QFileDialog>
#include <QCloseEvent>
#include <QProcess>
#include <QDomDocument>
#include <QTemporaryFile>
#include <QLibrary>
#include <QSpinBox>
#include <QMenu>

StaffWindow::StaffWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StaffWindow)
{
    ui->setupUi(this);    
    orgstruct_ = NULL;
    editPersonalWidget_ = NULL;

    connect(ui->actionOpen, &QAction::triggered, this, &StaffWindow::open_file);
    connect(ui->actionExit, &QAction::triggered, this, &StaffWindow::close);
    connect(ui->treeWidget_org, &QTreeWidget::itemClicked, this, &StaffWindow::current_org_changed);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &StaffWindow::current_tab_changed);
    connect(ui->actionSave, &QAction::triggered, this, &StaffWindow::save_file);
    connect(ui->actionInfo, &QAction::triggered, this, &StaffWindow::help);
    connect(ui->treeWidget_org, &QTreeWidget::itemChanged, this, &StaffWindow::edit_org);
    connect(ui->actionCalc, &QAction::triggered, this, &StaffWindow::calc_holidays);
    connect(ui->actionRap, &QAction::triggered, this, &StaffWindow::report);
    connect(ui->actionDataBase, &QAction::triggered, this, &StaffWindow::inDataBase);
    connect(ui->actionFromDB, &QAction::triggered, this, &StaffWindow::fromDataBase);

    ui->splitter->setStretchFactor(0,3);
    ui->splitter->setStretchFactor(1,10);
    QString labelText = QString::fromUtf8("<p style=\" font-size:16pt; \"><b>%1</b></p>").arg(QDate::currentDate().toString("dd.MM.yyyy"));
    QLabel* currentDateLabel = new QLabel(labelText, this);
    currentDateLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    currentDateLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    currentDateLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(currentDateLabel);
    ui->tableWidget_Personal->clearContents();
    ui->tableWidget_Personal->setRowCount(0);
    ui->treeWidget_org->clear();
    ui->tableWidget_Personal->resizeColumnsToContents();
    ui->tabWidget->setCurrentWidget(ui->tab_person);


//    QMenu* openMenu = new QMenu();
//    openMenu->setIcon(QIcon(":/icon/user-desktop-symbolic.symbolic.png"));
//    QAction* openFile = new QAction(QString::fromUtf8("Из файла"), this);
//    openMenu->addAction(openFile);
//    connect(openFile, &QAction::triggered, this, &StaffWindow::open_file);
//    ui->actionOpen->setMenu(openMenu);
//    ui->mainToolBar->insertAction(ui->actionSave, openMenu->menuAction());
//    openMenu->setContextMenuPolicy(Qt::DefaultContextMenu);


    QAction* addOrgAct = new QAction(QString::fromUtf8("Организацию"), this);
    QAction* addSubAct = new QAction(QString::fromUtf8("Отдел"), this);
    ui->toolButton_AddOrg->addAction(addOrgAct);
    ui->toolButton_AddOrg->addAction(addSubAct);
    connect(addOrgAct, &QAction::triggered, this, &StaffWindow::on_toolButton_AddOrg_clicked);
    connect(addSubAct, &QAction::triggered, this, &StaffWindow::AddSub);

    set_buttons_enabled(false, false);

}

StaffWindow::~StaffWindow()
{
    delete ui;
}

void StaffWindow::on_toolButton_AddPer_clicked()
{ 
    QUuid subKey = ui->treeWidget_org->currentItem()->data(0, Qt::UserRole).toUuid();
    QUuid orgKey = orgstruct_->subs().value(subKey)->orgKey();
    show_form(NULL, orgKey, subKey);
    qDebug() << orgKey << "---" << subKey;
}

void StaffWindow::on_toolButton_EditPer_clicked()
{
    int curRow = ui->tableWidget_Personal->currentRow();
    QUuid key = ui->tableWidget_Personal->item(curRow, TABID)->data(Qt::UserRole).toUuid();
//    QUuid orgKey = ui->listWidget_Org->currentItem()->data(Qt::UserRole).toUuid();

    Person* curPer = orgstruct_->getPerson(key);
//    qDebug() << curPer->toString();

    show_form(curPer, NULL, NULL);

    //ui->statusBar->showMessage(QString::fromUtf8("отредактировали сотрудника"),2000);
}

void StaffWindow::on_toolButton_DelPer_clicked()
{
    QTreeWidgetItem *orgItem = ui->treeWidget_org->currentItem();
    QUuid orgKey = orgItem->data(0, Qt::UserRole).toUuid();

    int curPerRow = ui->tableWidget_Personal->currentRow();
    QUuid key = ui->tableWidget_Personal->item(curPerRow, TABID)->data(Qt::UserRole).toUuid();
    Person* curPer = orgstruct_->takePerson(key);

    delete curPer;

    fill_pers(orgKey);


    //ui->statusBar->showMessage(QString::fromUtf8("удалили сотрудника"),2000);
}

void StaffWindow::on_toolButton_DelOrg_clicked()
{

    QTreeWidgetItem *orgItem = ui->treeWidget_org->currentItem();
//    qDebug() << orgItem->text();
    QUuid orgUuid = orgItem->data(0,Qt::UserRole).toUuid();
    qDebug() << orgUuid;
    if (orgstruct_->organizations().contains(orgUuid)){
        Organization* org = orgstruct_->takeOrganization(orgUuid);
        delete org;
        ui->treeWidget_org->takeTopLevelItem(ui->treeWidget_org->indexOfTopLevelItem(orgItem));
        delete orgItem;

         qDebug() << ui->treeWidget_org->indexOfTopLevelItem(orgItem);
    } else if (orgstruct_->subs().contains(orgUuid)) {
        Subdivizions* sub = orgstruct_->takeSubdiv(orgUuid);
        delete sub;
//        orgItem->parent()->takeChild(orgItem->parent()->indexOfChild(orgItem));
//        delete orgItem;
        ui->treeWidget_org->clear();
        fill_orgs(NULL, 0);
//        qDebug() << orgItem->parent()->indexOfChild(orgItem);
    }
}

void StaffWindow::on_toolButton_AddOrg_clicked()
{
    QString name = QString::fromUtf8("Новая организация");
    QUuid uuid = QUuid::createUuid();
    Organization* org = new Organization();
    org->setKey(uuid);
    org->setName(name);
    orgstruct_->addOrganization(org);

//    sub->setOrgKey(org->key());

    QTreeWidgetItem *orgItem = new QTreeWidgetItem();
    orgItem->setText(0, org->name());
    orgItem->setData(0,Qt::UserRole,org->key());
    orgItem->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled|Qt::ItemIsSelectable);
    ui->treeWidget_org->addTopLevelItem(orgItem);
    ui->treeWidget_org->editItem(orgItem);
    ui->treeWidget_org->setCurrentItem(orgItem);

}


void StaffWindow::open_file()
{
    QString xml_doc = QFileDialog::getOpenFileName(this,QString::fromUtf8("Открыть файл"), QApplication::applicationFilePath(), "*.xml");
    OrgStruct* orgBack = orgstruct_;
    orgstruct_ = new OrgStruct();
    QString erMsg = orgstruct_->loadFromXML(xml_doc);
    ui->treeWidget_org->clear();
    if (!erMsg.isEmpty())
    {
        ui->statusBar->showMessage(erMsg,4000);
        if (orgstruct_) delete orgstruct_;
        orgstruct_ = orgBack;
        fill_orgs(NULL, 0);
        if (orgstruct_) set_buttons_enabled(true, false);
        return;
    }
    if (orgBack) delete orgBack;    
    fill_orgs(NULL, 0);
    set_buttons_enabled(true, false);

    //вызов заполнения списка
}

void StaffWindow::save_file()
{
    QString msg;
    if (!orgstruct_)
    {
        msg = QString::fromUtf8("Ошибка при сохранении данных");
        qWarning() << msg;
        ui->statusBar->showMessage(msg, 4000);
        return;
    }
     QString xml_doc = QFileDialog::getSaveFileName(this,QString::fromUtf8("Сохранить файл"), QApplication::applicationFilePath()+"/organizations.xml", "XML files (*.xml)");
     if (orgstruct_->toXML(xml_doc))
     {
         msg = QString::fromUtf8("Файл %1 успешно сохранен").arg(xml_doc);
         qDebug() << msg;
         ui->statusBar->showMessage(msg, 4000);
     } else {
         msg = QString::fromUtf8("Ошибка при сохранении файла %1").arg(xml_doc);
         qWarning() << msg;
         ui->statusBar->showMessage(msg, 4000);
     }
}

void StaffWindow::report()
{
    QString msg;
    if (!orgstruct_)
    {
        msg = QString::fromUtf8("Ошибка при формировании отчёта");
        qWarning() << msg;
        ui->statusBar->showMessage(msg, 4000);
        return;
    }
    QString xml_doc = QFileDialog::getSaveFileName(this,QString::fromUtf8("Сохранить файл"), QApplication::applicationFilePath()+"/report.rtf", "RTF files (*.rtf)");
    if (orgstruct_->toXMLReport(xml_doc))
    {
        msg = QString::fromUtf8("Файл %1 успешно сохранен").arg(xml_doc);
        qDebug() << msg;
        ui->statusBar->showMessage(msg, 4000);
    } else {
        msg = QString::fromUtf8("Ошибка при сохранении файла %1").arg(xml_doc);
        qWarning() << msg;
        ui->statusBar->showMessage(msg, 4000);
    }

//    QProcess::execute(QString("firefox"), QStringList()<< xml_doc);

}

void StaffWindow::fill_orgs(QUuid orgKey, QTreeWidgetItem *parent)
{
    if (!orgstruct_) { return; }
    QTreeWidgetItem *item = 0;
    foreach (const Organization* org, orgstruct_->organizations()) {
        if (parent){ // отделы
            if (orgstruct_->subs().isEmpty()) return;
            foreach (const Subdivizions* sub, orgstruct_->subs()) {
                if (sub->orgKey()==orgKey){
                    item = new QTreeWidgetItem();
                    item->setText(0, sub->name());
                    item->setData(0, Qt::UserRole, sub->key());
                    item->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled|Qt::ItemIsSelectable);
                    parent->addChild(item);
                }
            }            
            orgKey = NULL;
        } else { // организации
            item = new QTreeWidgetItem();
            item->setText(0, org->name());
            item->setData(0, Qt::UserRole,org->key());
            item->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled|Qt::ItemIsSelectable);
            ui->treeWidget_org->addTopLevelItem(item);
//            ui->treeWidget_org->contextMenuEvent(menu);
            orgKey = org->key();
            fill_orgs(orgKey, item);
        }
    }
    ui->treeWidget_org->sortItems(0, Qt::AscendingOrder);
}

void StaffWindow::calc_holidays()
{
    QString msg;
    if (!orgstruct_)
    {
        msg = QString::fromUtf8("Ошибка при рассчете данных");
        qWarning() << msg;
        ui->statusBar->showMessage(msg, 4000);
        return;
    }
    QDomDocument doc;
    doc.appendChild(doc.createProcessingInstruction(QString("xml"), QString("version = \"1.0\" encoding = \"UTF-8\"")));
    QDomElement holidays = doc.createElement("Holidays");
    doc.appendChild(holidays);

    for (int i = 0; i < ui->tableWidget_holiday->rowCount(); ++i) {
        QUuid perKey = ui->tableWidget_holiday->item(i, HC_TABID)->data(Qt::UserRole).toUuid();
//        QUuid orgKey = ui->listWidget_Org->currentItem()->data(Qt::UserRole).toUuid();

        Person* curPer = orgstruct_->getPerson(perKey);
        if(!curPer) continue;
        if(!curPer->holiday()) continue;

        QDomElement person = doc.createElement("Person");
        QDomAttr per_uuid = doc.createAttribute("UUID");
        per_uuid.setNodeValue(perKey.toString());
        person.setAttributeNode(per_uuid);
        holidays.appendChild(person);

        QDomElement beginDate = doc.createElement("Begin_Date");
        QDomText t = doc.createTextNode(curPer->holiday()->begin().toString("dd.MM.yyyy"));
        beginDate.appendChild(t);
        person.appendChild(beginDate);

        QDomElement duration = doc.createElement("Duration");
        t = doc.createTextNode(QString::number(curPer->holiday()->duration()));
        duration.appendChild(t);
        person.appendChild(duration);

    }
    qDebug().noquote() << doc.toString();

    QTemporaryFile file(QString("%1/calcHolidayXXXXXXXXXX.xml").arg(QApplication::applicationDirPath()));
//    file.setAutoRemove(false);
    //    QFile file(QString("%1/calcHoliday.xml").arg(QApplication::applicationDirPath()));
    //    if (file.open(QIODevice::WriteOnly|QIODevice::Text))
    if (file.open())
    {
        qDebug() << file.fileName();
        file.write(doc.toByteArray(3));
        file.close();
    }
    QString fileName = file.fileName();
    QLibrary holidayLib(QString("%1/libholiday.so").arg(QApplication::applicationDirPath()));
    typedef int (*Function_evaluate_holiday)(QString &);
    Function_evaluate_holiday function_evaluate_holiday;
    int exitStatus = 0;

    QUuid orgKey = ui->treeWidget_org->currentItem()->data(0, Qt::UserRole).toUuid();

    if (holidayLib.load()) {
        qDebug() << "holidayLib.load()";
        function_evaluate_holiday = (Function_evaluate_holiday) holidayLib.resolve("evaluate_holiday");
        if (function_evaluate_holiday) {
            exitStatus = function_evaluate_holiday(fileName);
            if (exitStatus == 0) {
                qDebug() << QString::fromUtf8("Все хорошо");
                from_XML_holidays(fileName);
                fill_holiday_table(orgKey);
            } else {
                qDebug() << QString::fromUtf8("Не удалось отправить xml на сервер. Расчет не выполнен! Обратитесь к администратору.");
            }
        }
    }
}

void StaffWindow::current_tab_changed(int idxTab)
{
    if (!orgstruct_) { return; }
    if (idxTab != ui->tabWidget->indexOf(ui->tab_holiday)) { return; }
    QUuid orgKey = ui->treeWidget_org->currentItem()->data(0, Qt::UserRole).toUuid();

    fill_holiday_table(orgKey);
}

void StaffWindow::current_org_changed(QTreeWidgetItem *OrgItem)
{
    if (!orgstruct_) { return; }
//    if(ui->treeWidget_org->currentItem() != OrgItem) return;
//    ui->treeWidget_org->setCurrentItem(OrgItem);
    if(OrgItem->parent()) set_buttons_enabled(true, true); else set_buttons_enabled(true, false);
    QUuid orgKey = OrgItem->data(0, Qt::UserRole).toUuid();
    qDebug() << "current_org_changed" << orgKey;
    fill_pers(orgKey);
    fill_holiday_table(orgKey);
}

void StaffWindow::fill_pers(QUuid orgKey)
{
    if (!orgstruct_) { return; }    
    ui->tableWidget_Personal->clearContents();
    ui->tableWidget_Personal->setRowCount(0);
    int rowCount = 0;
     //qDebug()<< "in orgKey" << orgKey;
    foreach (const Person* per, orgstruct_->staff().values()) {
//        qDebug()<< "in foreach" << per->orgKey() << "----" << orgKey;
        if (orgstruct_->subs().contains(per->subdivizionKey())){
            if (per->orgKey() == orgKey || per->subdivizionKey() == orgKey)
            {
                rowCount++;
                ui->tableWidget_Personal->setRowCount(rowCount);

                QTableWidgetItem *newTabID = new QTableWidgetItem(QString::number(per->tabID()));
                ui->tableWidget_Personal->setItem(rowCount-1, TABID, newTabID);
                newTabID->setData(Qt::UserRole, per->key());

                QTableWidgetItem *newFIO = new QTableWidgetItem(per->FIO());
                ui->tableWidget_Personal->setItem(rowCount-1, FIO, newFIO);

                if (per->orgKey() == orgKey){
                    ui->tableWidget_Personal->setColumnCount(6);
                    QStringList lst;
                    lst << "Табельный №" << "ФИО"  << "Пол" << "Дата приема на работу" << "Стаж" << "Отдел";
                    ui->tableWidget_Personal->setHorizontalHeaderLabels(lst);
                    QTableWidgetItem *newSub = new QTableWidgetItem(orgstruct_->subs().value(per->subdivizionKey())->name());
                    ui->tableWidget_Personal->setItem(rowCount-1, SUBDIV, newSub);
                } else {
                    ui->tableWidget_Personal->setColumnCount(5);
                    QStringList lst;
                    lst << "Табельный №" << "ФИО" << "Пол" << "Дата приема на работу" << "Стаж";
                    ui->tableWidget_Personal->setHorizontalHeaderLabels(lst);
                }

                QTableWidgetItem *newSex = new QTableWidgetItem((per->sex()==0)? QString::fromUtf8("м") : QString::fromUtf8("ж"));
                ui->tableWidget_Personal->setItem(rowCount-1, SEX, newSex);

                QTableWidgetItem *newDate = new QTableWidgetItem(per->empDate().toString("dd.MM.yyyy"));
                ui->tableWidget_Personal->setItem(rowCount-1, DATE, newDate);

                QTableWidgetItem *newSenr = new QTableWidgetItem(per->seniorityString());
                ui->tableWidget_Personal->setItem(rowCount-1, SENIORITY, newSenr);

    //            qDebug() << "fill pers:" << orgstruct_->toString_per() << "/n";
            }
        }

    }
    ui->tableWidget_Personal->resizeColumnsToContents();
    ui->tableWidget_Personal->sortByColumn(TABID, Qt::AscendingOrder);
    ui->tableWidget_Personal->setCurrentItem(ui->tableWidget_Personal->item(0,0)); // TODO
}

void StaffWindow::edit_org(QTreeWidgetItem *itemOrg)
{
    QUuid orgKey = itemOrg->data(0, Qt::UserRole).toUuid();
    QString orgName = itemOrg->text(0);

    if (orgstruct_->organizations().contains(orgKey)){
        Organization* org = orgstruct_->getOrganization(orgKey);
//        if(!org) return;
        org->setName(orgName);
        qDebug().noquote()<< "name changed" << org->toString();
    } else if (orgstruct_->subs().contains(orgKey)) {
        Subdivizions* sub = orgstruct_->getSubdiv(orgKey);
        sub->setName(orgName);
    } else return;
//    ui->treeWidget_org->setCurrentRow(ui->listWidget_Org->row(itemOrg));

//    emit ui->listWidget_Org->currentRowChanged(ui->listWidget_Org->row(itemOrg));
}

void StaffWindow::set_buttons_enabled(bool enableorg, bool enableper)
{
    ui->toolButton_AddOrg->setEnabled(enableorg);
    ui->toolButton_AddPer->setEnabled(enableper);
    ui->toolButton_DelOrg->setEnabled(enableorg);
    ui->toolButton_DelPer->setEnabled(enableper);
    ui->toolButton_EditPer->setEnabled(enableper);
}

void StaffWindow::person_edited(Person *person, bool isNew)
{
    if (person == NULL) { return; }
    qDebug() << "person edbted" << person->toString();
    QUuid subKey = ui->treeWidget_org->currentItem()->data(0, Qt::UserRole).toUuid();
    QUuid orgKey = orgstruct_->subs().value(subKey)->orgKey();
    if (isNew)
    {
        orgstruct_->addPerson(person);
        person->setOrgKey(orgKey);
    }    
    fill_pers(subKey);
}

void StaffWindow::help()
{
    QString fileName = QApplication::applicationDirPath() + "/../doxy/doc/html/index.html";
    qDebug() << fileName;

    if(!QFile::exists(fileName)) {
        ui->statusBar->showMessage(QString::fromUtf8("Файл справки не найден"), 2000);
        return;
    }
    QProcess::execute(QString("firefox"), QStringList()<<fileName);
}

void StaffWindow::fill_holiday_table(QUuid orgKey)
{
    QColor grey = QColor::fromRgb(240, 240, 240);    
    ui->tableWidget_holiday->clearContents();
    ui->tableWidget_holiday->setRowCount(0);

    int rowCount = 0;
    foreach (const Person* per, orgstruct_->staff().values()) {
        if (orgstruct_->subs().contains(per->subdivizionKey())){
            if (per->orgKey() == orgKey||per->subdivizionKey() == orgKey)
            {
                rowCount++;
                ui->tableWidget_holiday->setRowCount(rowCount);
                QTableWidgetItem *newItem = new QTableWidgetItem(QString::number(per->tabID()));
                newItem->setFlags(Qt::NoItemFlags);
                newItem->setBackgroundColor(grey);
                newItem->setTextColor(Qt::black);
                newItem->setData(Qt::UserRole, per->key());

                ui->tableWidget_holiday->setItem(rowCount-1, HC_TABID, newItem);

                newItem = new QTableWidgetItem(per->FIO());
                newItem->setFlags(Qt::NoItemFlags);
                newItem->setBackgroundColor(grey);
                newItem->setTextColor(Qt::black);
                ui->tableWidget_holiday->setItem(rowCount-1, HC_FIO, newItem);

                newItem = new QTableWidgetItem(orgstruct_->subs().value(per->subdivizionKey())->name());
                newItem->setFlags(Qt::NoItemFlags);
                newItem->setBackgroundColor(grey);
                newItem->setTextColor(Qt::black);
                ui->tableWidget_holiday->setItem(rowCount-1, HC_SUBDIV, newItem);

                QString text;
                if (per->holiday())
                {
                    text = per->holiday()->begin().toString("dd.MM.yyyy");
                }
                newItem = new QTableWidgetItem(text);
                newItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled );
                newItem->setTextColor(Qt::black);
                ui->tableWidget_holiday->setItem(rowCount-1,HC_BEGINDATE,newItem);

                if (per->holiday())
                {
                    text = QString::number(per->holiday()->duration());

                } else {
                    text = "";
                }
                newItem = new QTableWidgetItem(text);
                newItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled );
                newItem->setTextColor(Qt::black);
                ui->tableWidget_holiday->setItem(rowCount-1,HC_DURATION,newItem);

                if (per->holiday())
                {
                    text = per->holiday()->endString();
                } else {
                    text = "";
                }
                newItem = new QTableWidgetItem(text);
                newItem->setFlags(Qt::NoItemFlags);
                newItem->setBackgroundColor(QColor::fromRgb(240, 255, 240));
                newItem->setTextColor(Qt::black);
                ui->tableWidget_holiday->setItem(rowCount-1,HC_ENDDATE,newItem);
            }
        }


    }
    ui->tableWidget_holiday->resizeColumnsToContents();
    ui->tableWidget_holiday->sortByColumn(TABID, Qt::AscendingOrder);
    ui->tableWidget_holiday->setCurrentItem(ui->tableWidget_holiday->item(0,0)); // TODO
    ui->tableWidget_holiday->setItemDelegateForColumn(HC_DURATION, new SpinBoxDelegate);
    ui->tableWidget_holiday->setItemDelegateForColumn(HC_BEGINDATE, new DateEditDelegate);
}

void StaffWindow::show_form(Person *person, QUuid orgKey, QUuid subKey)
{
    if (editPersonalWidget_) { delete editPersonalWidget_; }
    qDebug() << "show_form" << orgKey << "---" << subKey;
    editPersonalWidget_ = new EditPersonalWidget(person, orgKey, subKey);
    connect(editPersonalWidget_, &EditPersonalWidget::person_edited, this, &StaffWindow::person_edited);
    editPersonalWidget_->setWindowModality(Qt::ApplicationModal);
    editPersonalWidget_->show();
}




void StaffWindow::on_tableWidget_holiday_itemChanged(QTableWidgetItem *item)
{
      if (!orgstruct_) { return; }
    QUuid perKey = ui->tableWidget_holiday->item(item->row(), HC_TABID)->data(Qt::UserRole).toUuid();

    Person* curPer = orgstruct_->getPerson(perKey);
    if (!curPer) return;
// qDebug() << item->row()<< item->column() << item->text();
    if (item->column() == HC_BEGINDATE) {

        QDate begDate = QDate::fromString(item->text(), "dd.MM.yyyy");
        if (begDate.isValid())
        {
            if (!curPer->holiday()) { Holiday* holiday = new Holiday(); curPer->addHoliday(holiday);}
            curPer->holiday()->setBegin(begDate);
        }
    }
    else if (item->column() == HC_DURATION) {

        bool ok;
        uint dur = item->text().toUInt(&ok);
        if(ok)
        {
            if (!curPer->holiday()) { Holiday* holiday = new Holiday(); curPer->addHoliday(holiday);}
            curPer->holiday()->setDuration(dur);
        }
        }
    if(curPer->holiday()) {
        qDebug() << curPer->holiday()->begin().toString();
        qDebug() << curPer->holiday()->duration();
    }

}

void StaffWindow::from_XML_holidays(QString fileName)
{
    QString msg;
    QDomDocument domDoc;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        msg = QString::fromUtf8("Ошибка при открытии файла %1").arg(fileName);
        qWarning() << msg;
        return;
    }

    if(!domDoc.setContent(&file))
    {
        msg = QString::fromUtf8("Ошибка при чтении файла %1").arg(fileName);
        qWarning() << msg;
        file.close();
        return;
    }
    file.close();

    qDebug().noquote() << domDoc.toString(3);

    QUuid orgKey = ui->treeWidget_org->currentItem()->data(0,Qt::UserRole).toUuid();
    QDomElement e_person = domDoc.firstChildElement().firstChildElement("Person");
    while(!e_person.isNull())
    {
//        qDebug() <<e_person.tagName() <<e_person.attribute("UUID")<< e_person.firstChildElement("Name").text() ;
        Holiday* holiday = new Holiday();
        QUuid key = e_person.attribute("UUID");
        QDate date = QDate::fromString(e_person.firstChildElement("Begin_Date").text(), "dd.MM.yyyy");
        holiday->setBegin(date);
        holiday->setDuration(e_person.firstChildElement("Duration").text().toUInt());
        QDate endDateTxt = QDate::fromString(e_person.firstChildElement("End_Date").text(), "dd.MM.yyyy");
        if (endDateTxt.isValid())
        {
            holiday->setEnd(new QDate(endDateTxt));
        }
        foreach (Person* per, orgstruct_->staff().values()) {
            if(per->key() == key && (per->orgKey() == orgKey || per->subdivizionKey() == orgKey)){
                per->addHoliday(holiday);
            }
        }

        //            holidays_.insert(key, holiday);
        e_person = e_person.nextSiblingElement("Person");
//        qDebug() << "from_XML_holidays: " << key << holiday->begin().toString() << holiday->duration() << holiday->end()->toString();
    }
//    qDebug() << "from_XML_holidays: " << pers->toString();

}

void StaffWindow::AddSub()
{
    QUuid orgKey;
    QTreeWidgetItem* orgItem;
    if(ui->treeWidget_org->currentItem()->parent()){
        QUuid subKey = ui->treeWidget_org->currentItem()->data(0, Qt::UserRole).toUuid();
        orgKey = orgstruct_->subs().value(subKey)->orgKey();
        orgItem = ui->treeWidget_org->currentItem()->parent();
//        orgItem = ui->treeWidget_org->topLevelItem(ui->treeWidget_org->indexOfTopLevelItem());
    } else {
        orgKey = ui->treeWidget_org->currentItem()->data(0, Qt::UserRole).toUuid();
        orgItem = ui->treeWidget_org->currentItem();
    }

    Subdivizions* sub = new Subdivizions(orgKey);
    QString name = QString::fromUtf8("Новый отдел");
    QUuid uuid = QUuid::createUuid();
    sub->setKey(uuid);
    sub->setName(name);
    orgstruct_->addSubdiv(sub);

    QTreeWidgetItem *subItem = new QTreeWidgetItem();
    subItem->setText(0, sub->name());
    subItem->setData(0,Qt::UserRole,sub->key());
    subItem->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled|Qt::ItemIsSelectable);
    orgItem->addChild(subItem);
}

void StaffWindow::fromDataBase()
{

    OrgStruct* orgBack = orgstruct_;
    orgstruct_ = new OrgStruct();
    ui->treeWidget_org->clear();
    if (!orgstruct_->fromDB())
    {
        ui->statusBar->showMessage(QString::fromUtf8("Ошибка при загрузке данных"),4000);
        if (orgstruct_) delete orgstruct_;
        orgstruct_ = orgBack;
        fill_orgs(NULL, 0);
        if (orgstruct_) set_buttons_enabled(true, false);
        return;
    }
    if (orgBack) delete orgBack;
    fill_orgs(NULL, 0);
    set_buttons_enabled(true, false);

}



void StaffWindow::inDataBase()
{
    if (!orgstruct_) return;
    qDebug() << "in database";
    orgstruct_->toDB();
}
