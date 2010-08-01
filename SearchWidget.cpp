#include "SearchWidget.h"
#include "ui_SearchWidget.h"

#include <QMessageBox>
#include <QRegExp>
#include <QTextStream>

SearchWidget::SearchWidget(const QString & searchExpression, bool isRegex, bool caseSensitive, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SearchWidget)
{
    ui->setupUi(this);

    ui->lineEdit_search->setText(searchExpression);
    ui->lineEdit_search->selectAll();
    ui->checkBox_regex->setChecked(isRegex);
    ui->checkBox_caseSensitive->setChecked(caseSensitive);
}

SearchWidget::~SearchWidget()
{
    delete ui;
}

void SearchWidget::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void SearchWidget::accept()
{
    if(ui->checkBox_regex->isChecked()) {
        QRegExp regex(ui->lineEdit_search->text(),
            ui->checkBox_caseSensitive->isChecked() ? Qt::CaseSensitive: Qt::CaseInsensitive,
            QRegExp::RegExp2);
        if(!regex.isValid()) {
            QString message;
            QTextStream(&message)
                << tr("The pattern \"")
                << ui->lineEdit_search->text()
                << tr("\" is invalid.");
            QMessageBox::critical(this, "yata", message);
            ui->lineEdit_search->setFocus();
            return;
        }
    }
    emit searchAccepted(ui->lineEdit_search->text(), ui->checkBox_regex->isChecked(), ui->checkBox_caseSensitive->isChecked());
    QDialog::accept();
}
