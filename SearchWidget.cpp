/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 * 
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "SearchWidget.h"
#include "ui_SearchWidget.h"

#include "SearchInfo.h"

#include <QMessageBox>
#include <QRegExp>
#include <QTextStream>

#include "YApplication.h"

SearchWidget::SearchWidget(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SearchWidget)
{
    ui->setupUi(this);

    const SearchCriteria & sc = SearchInfo::instance().search();

    ui->lineEdit_search->setText(sc.expression);
    ui->lineEdit_search->selectAll();
    ui->checkBox_regex->setChecked(sc.isRegex);
    ui->checkBox_caseSensitive->setChecked(sc.isCaseSensitive);
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
            QMessageBox::critical(this, YApplication::displayAppName(), message);
            ui->lineEdit_search->setFocus();
            return;
        }
    }
    SearchCriteria sc(
        ui->lineEdit_search->text(),
        ui->checkBox_regex->isChecked(),
        ui->checkBox_caseSensitive->isChecked());
    SearchInfo::instance().setSearch(sc);
    QDialog::accept();
}
