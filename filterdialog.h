#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include "suicidemodel.h"

namespace Ui {
class FilterDialog;
}

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterDialog(SuicideSortFilterProxyModel *filterModel, QWidget *parent = nullptr);
    ~FilterDialog();

    void setFilterFixedString(const QString &pattern);
    void removeFilter(QString value);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::FilterDialog *ui;
    FilterRequest _request;
    SuicideSortFilterProxyModel* _filterModel;
};

#endif // FILTERDIALOG_H
