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
