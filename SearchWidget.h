#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QDialog>

namespace Ui {
    class SearchWidget;
}


class SearchWidget : public QDialog {
    Q_OBJECT
public:
    SearchWidget(const QString & searchExpression, bool isRegex, bool caseSensitive, QWidget *parent = 0);
    ~SearchWidget();
signals:
    void searchAccepted(const QString & searchExpression, bool isRegex, bool caseSensitive);

public slots:
    void accept();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SearchWidget *ui;
};

#endif // SEARCHWIDGET_H
