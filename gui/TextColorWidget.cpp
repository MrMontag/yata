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
    ui(new Ui::TextColorWidget)
{
    ui->setupUi(this);
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

TextColor TextColorWidget::textColor() const
{
    return TextColor(ui->foregroundColor->currentColor(), ui->backgroundColor->currentColor());
}

void TextColorWidget::setTextColor(const TextColor & tc)
{
    ui->foregroundColor->setCurrentColor(tc.foreground());
    ui->backgroundColor->setCurrentColor(tc.background());
}
