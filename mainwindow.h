#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "suicidemodel.h"
#include "filterdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void enableAllButtons(bool state = true);

private slots:
    void on_actionOpen_triggered();
    void onTableDataChanged();
    void onFilterAccepted();

    void on_DeleteRowButton_clicked();

    void on_actionSave_As_triggered();

    void on_RefreshDataButton_clicked();

    void on_actionSave_triggered();

    void on_InsertRowButton_clicked();

    void on_DeselectAllButton_clicked();

    void on_FilterButton_clicked();

    void on_ResetFilterButton_clicked();

private:
    Ui::MainWindow *ui;
    FilterDialog *filterView;
    SuicideSortFilterProxyModel *proxyModel;
    SuicideModel* _model;
    // Suicides* _storage;
};
#endif // MAINWINDOW_H
