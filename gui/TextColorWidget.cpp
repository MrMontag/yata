/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
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
