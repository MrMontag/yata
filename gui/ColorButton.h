/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */

#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QPushButton>

class ColorButton : public QPushButton {
    Q_OBJECT

public:
    ColorButton(QWidget *parent = 0);
    ~ColorButton();

    const QColor & currentColor() const;
    void setCurrentColor(const QColor & color);

protected:
    virtual void paintEvent(QPaintEvent *e);
private slots:
    void onClick();
private:
    QColor m_currentColor;
};

#endif // COLORBUTTON_H
