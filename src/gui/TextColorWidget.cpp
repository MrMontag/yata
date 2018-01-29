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
#include "TextColorWidget.h"
#include "ui_TextColorWidget.h"
#include "preferences/TextColor.h"

TextColorWidget::TextColorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextColorWidget),
    m_textColor(new TextColor)
{
    ui->setupUi(this);
    connect(ui->foregroundColor, SIGNAL(currentColorChanged(QColor)), SLOT(currentColorChanged(QColor)));
    connect(ui->backgroundColor, SIGNAL(currentColorChanged(QColor)), SLOT(currentColorChanged(QColor)));
}

TextColorWidget::~TextColorWidget()
{
    delete ui;
}

QString TextColorWidget::text() const
{
    return ui->titleLabel->text();
}

void TextColorWidget::setText(const QString & text) const
{
    ui->titleLabel->setText(text);
}

const TextColor & TextColorWidget::textColor() const
{
    return *m_textColor;
}

void TextColorWidget::setTextColor(const TextColor & tc)
{
    *m_textColor = tc;
    updateColorButtons();
}

void TextColorWidget::on_defaultCheckBox_clicked(bool checked)
{
    m_textColor->setToDefault(checked);
    updateColorButtons();
}

void TextColorWidget::currentColorChanged(const QColor & color)
{
    if (sender() == ui->foregroundColor) {
        m_textColor->setForeground(color);
    } else {
        m_textColor->setBackground(color);
    }

    ui->defaultCheckBox->setChecked(false);
}

void TextColorWidget::updateColorButtons()
{
    ui->foregroundColor->setCurrentColor(m_textColor->foreground());
    ui->backgroundColor->setCurrentColor(m_textColor->background());
    ui->defaultCheckBox->setChecked(m_textColor->isDefault());
}
