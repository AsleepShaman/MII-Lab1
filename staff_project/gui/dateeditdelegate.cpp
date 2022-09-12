#include "dateeditdelegate.h"
#include <QDebug>
#include <QDateEdit>


DateEditDelegate::DateEditDelegate(QObject *parent)
      : QStyledItemDelegate(parent)
  {
  }

QWidget *DateEditDelegate::createEditor(QWidget *parent,
      const QStyleOptionViewItem &/* option */,
      const QModelIndex &/* index */) const
  {
      QDateEdit *editor = new QDateEdit(parent);
      editor->setFrame(false);
      editor->setCalendarPopup(true);
      editor->setMinimumDate(QDate::currentDate());
      editor->setMaximumDate(QDate::currentDate().addYears(1));
      return editor;
  }


void DateEditDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QDate value = index.model()->data(index, Qt::EditRole).toDate();

    QDateEdit *dateEdit = static_cast<QDateEdit*>(editor);

    dateEdit->setDate(value);
    qDebug() << "dateEdit->text()" << dateEdit->text();
}

void DateEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                     const QModelIndex &index) const
  {
      QDateEdit *dateEdit = static_cast<QDateEdit*>(editor);
//      dateEdit->interpretText();
      QDate value = dateEdit->date();

      model->setData(index, value.toString("dd.MM.yyyy"), Qt::EditRole);
      qDebug() << "dateEdit->date();" << value;
  }

void DateEditDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}


