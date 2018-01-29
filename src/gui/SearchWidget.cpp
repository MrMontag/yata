/*
 * Yata -- Yet Another Tail Application
 *
 * Copyright (C) 2010-2012 James Smith
 * Copyright (C) 2018  Alexander Fust
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "SearchWidget.h"
#include "ui_SearchWidget.h"
#include "search/SearchInfo.h"
#include "YApplication.h"

#include <QCompleter>
#include <QLineEdit>
#include <QMessageBox>
#include <QRegExp>
#include <QTextStream>

SearchWidget::SearchWidget(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SearchWidget)
{
    ui->setupUi(this);

    const SearchInfo & si = SearchInfo::instance();

    for(SearchInfo::const_iterator sc = si.begin(); sc != si.end(); ++sc) {
        ui->comboBox_search->addItem(sc->expression);
    }

    const SearchCriteria & sc = si.search();

    ui->comboBox_search->lineEdit()->selectAll();
    ui->comboBox_search->completer()->setCaseSensitivity(Qt::CaseSensitive);
    ui->checkBox_regex->setChecked(sc.isRegex);
    ui->checkBox_caseSensitive->setChecked(sc.isCaseSensitive);
}

SearchWidget::~SearchWidget()
{
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
        QRegExp regex(ui->comboBox_search->currentText(),
            ui->checkBox_caseSensitive->isChecked() ? Qt::CaseSensitive: Qt::CaseInsensitive,
            QRegExp::RegExp2);
        if(!regex.isValid()) {
            QString message;
            QTextStream(&message)
                << tr("The pattern \"")
                << ui->comboBox_search->currentText()
                << tr("\" is invalid.");
            QMessageBox::critical(this, YApplication::displayAppName(), message);
            ui->comboBox_search->setFocus();
            return;
        }
    }
    SearchCriteria sc(
        ui->comboBox_search->currentText(),
        ui->checkBox_caseSensitive->isChecked(),
        ui->checkBox_regex->isChecked());
    SearchInfo::instance().acceptNewSearch(sc);
    QDialog::accept();
}
