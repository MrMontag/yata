/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QScopedPointer>

namespace Ui {
    class PreferencesDialog;
}

class PreferencesDialog : public QDialog {
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();

private slots:
    void on_buttonBox_accepted();
    void on_fontComboBox_currentFontChanged(const QFont & font);

    void on_fontDefaultBox_clicked();

private:
    QList<int> pointSizes(const QFont & font) const;
    void updateFont(const QFont & font);
    QScopedPointer<Ui::PreferencesDialog> ui;
};

#endif // PREFERENCESDIALOG_H
