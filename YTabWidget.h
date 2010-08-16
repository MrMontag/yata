#ifndef YTABWIDGET_H
#define YTABWIDGET_H

#include <QTabWidget>

class QToolButton;

class YTabWidget : public QTabWidget {
Q_OBJECT
public:
    explicit YTabWidget(QWidget *parent = 0);

private slots:
    void on_tabCloseRequested(int index);
    void on_tabChooseMenuTriggered();
private:
    void tabInserted(int index);
    void tabRemoved(int index);
private:
    QToolButton * m_chooseTabButton;

};

#endif // YTABWIDGET_H
