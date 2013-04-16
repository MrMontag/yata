#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include "gui/YTabWidget.h"

#include <QWidget>
#include <QScopedPointer>
#include <QPointer>

namespace Ui {
    class DebugWindow;
}

class DebugWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DebugWindow(YTabWidget* tabWidget, QWidget *parent = 0);
    ~DebugWindow();

private slots:
    void currentDocumentChanged(int);
    void documentContentsChanged();

private:
    QScopedPointer<Ui::DebugWindow> ui;
    QPointer<YTabWidget> m_tabWidget;
};

#endif // DEBUGWINDOW_H
