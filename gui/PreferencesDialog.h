/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

namespace Ui {
    class PreferencesDialog;
    //template <class T> class QList;
}

class PreferencesDialog : public QDialog {
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();

private slots:
    void on_buttonBox_accepted();
    void on_fontComboBox_currentFontChanged(const QFont & font);

private:
    QList<int> pointSizes(const QFont & font) const;
    Ui::PreferencesDialog *ui;
};

#endif // PREFERENCESDIALOG_H
