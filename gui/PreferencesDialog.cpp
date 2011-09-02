/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */

#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"

#include "preferences/Preferences.h"
#include <QtDebug>

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);
    const QFont & font = Preferences::instance()->font();
    ui->fontComboBox->setCurrentFont(font);
    ui->boldCheckBox->setChecked(font.bold());
    ui->italicCheckBox->setChecked(font.italic());
    on_fontComboBox_currentFontChanged(font);
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::on_buttonBox_accepted()
{
    QFont font = ui->fontComboBox->currentFont();
    font.setBold(ui->boldCheckBox->isChecked());
    font.setItalic(ui->italicCheckBox->isChecked());
    font.setPointSize(ui->fontSizeComboBox->currentText().toInt());
    Preferences::instance()->setFont(font);
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
