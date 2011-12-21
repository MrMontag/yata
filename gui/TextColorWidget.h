/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */

#ifndef TEXTCOLORWIDGET_H
#define TEXTCOLORWIDGET_H

#include <QWidget>
#include <QScopedPointer>

namespace Ui {
    class TextColorWidget;
}

class TextColor;

class TextColorWidget : public QWidget {
    Q_OBJECT

public:
    explicit TextColorWidget(QWidget *parent = 0);
    ~TextColorWidget();

    QString text() const;
    void setText(const QString & text) const;

    TextColor textColor() const;
    void setTextColor(const TextColor & tc);

    void setDefaultTextColor(const TextColor & dtc);

private slots:
    void on_defaultButton_clicked();

private:
    Ui::TextColorWidget *ui;
    QScopedPointer<TextColor> m_defaultTextColor;
};

#endif // TEXTCOLORWIDGET_H
