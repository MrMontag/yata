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
#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"

#include "preferences/Preferences.h"
#include "preferences/TextColor.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);
    ui->normalTextColor->setText(tr("Normal Text:"));
    ui->selectedTextColor->setText(tr("Selection:"));

    Preferences * pref = Preferences::instance();

    updateFont(pref->font());
    ui->normalTextColor->setTextColor(pref->normalTextColor());
    ui->selectedTextColor->setTextColor(pref->selectedTextColor());
}

PreferencesDialog::~PreferencesDialog()
{
}

void PreferencesDialog::on_buttonBox_accepted()
{
    QFont font = ui->fontComboBox->currentFont();
    font.setBold(ui->boldCheckBox->isChecked());
    font.setItalic(ui->italicCheckBox->isChecked());
    font.setPointSize(ui->fontSizeComboBox->currentText().toInt());
    Preferences * pref = Preferences::instance();
    pref->setFont(font);
    pref->setNormalTextColor(ui->normalTextColor->textColor());
    pref->setSelectedTextColor(ui->selectedTextColor->textColor());
    pref->write();
}

void PreferencesDialog::on_fontComboBox_currentFontChanged(const QFont & font)
{
    bool ok = false;
    int current = ui->fontSizeComboBox->currentText().toInt(&ok);
    if (!ok) {
        current = font.pointSize();
    }

    ui->fontSizeComboBox->clear();
    QList<int> pointSizeList = pointSizes(font);

    int newCurrentIndex = -1;
    for (int i = 0; i < pointSizeList.size(); i++) {
        ui->fontSizeComboBox->addItem(QString::number(pointSizeList[i]));
        if (newCurrentIndex == -1 && pointSizeList[i] >= current) {
            newCurrentIndex = i;
        }
    }
    if (newCurrentIndex != -1) {
        ui->fontSizeComboBox->setCurrentIndex(newCurrentIndex);
    }
}

// The following function was taken and modified from the source for QtCreator
QList<int> PreferencesDialog::pointSizes(const QFont & font) const
{
    QFontDatabase db;
    const QString familyName = font.family();
    QList<int> sizeLst = db.pointSizes(familyName);
    if (!sizeLst.isEmpty())
        return sizeLst;

    QStringList styles = db.styles(familyName);
    if (!styles.isEmpty()) {
        sizeLst = db.pointSizes(familyName, styles.first());
    }
    if (sizeLst.isEmpty()) {
        sizeLst = QFontDatabase::standardSizes();
    }

    return sizeLst;
}

void PreferencesDialog::on_fontDefaultBox_clicked()
{
    updateFont(QFont());
}

void PreferencesDialog::updateFont(const QFont & font)
{
    ui->fontComboBox->setCurrentFont(font);
    ui->boldCheckBox->setChecked(font.bold());
    ui->italicCheckBox->setChecked(font.italic());
    ui->fontSizeComboBox->clear();
    on_fontComboBox_currentFontChanged(font);
}
