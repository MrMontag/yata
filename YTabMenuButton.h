#ifndef WIDGET_H
#define WIDGET_H

#include <QToolButton>

class YTabMenuButton : public QToolButton {
public:
    YTabMenuButton(QWidget *parent = 0);
    ~YTabMenuButton();
protected:
    void paintEvent(QPaintEvent *);
};

#endif // WIDGET_H
