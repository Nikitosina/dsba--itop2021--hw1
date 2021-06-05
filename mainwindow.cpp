#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "filterdialog.h"

#include <QFileDialog>
#include <QDebug>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->DeleteRowButton->setIcon(QIcon(":/images/icons8-delete_sign.png"));
    ui->RefreshDataButton->setIcon(QIcon(":/images/icons8-refresh.png"));
    ui->FilterButton->setIcon(QIcon(":/images/icons8-filter.png"));
    ui->ResetFilterButton->setIcon(QIcon(":/images/icons8-clear_filters.png"));
    ui->DeselectAllButton->setIcon(QIcon(":/images/icons8-close_window.png"));
    ui->InsertRowButton->setIcon(QIcon(":/images/icons8-thick_arrow_pointing_down.png"));

//    QLabel *label = new QLabel();
//    label->setText("123123123");
//    filterView->layout()->addWidget(label);
//    filterView->resize(100, 100);

    _model = new SuicideModel(this);
    proxyModel = new SuicideSortFilterProxyModel(this);
    proxyModel->setSourceModel( _model );
    proxyModel->disableFilter();
    // _storage = new Suicides(_model, this);

    ui->tableView->setModel(proxyModel);
    ui->tableView->verticalHeader()->show();
    ui->tableView->setColumnWidth(0, 100);
    ui->tableView->setColumnWidth(1, 50);
    ui->tableView->setColumnWidth(3, 160);
    ui->tableView->setColumnWidth(6, 50);

    enableAllButtons(false);

    QObject::connect(_model, &SuicideModel::tableDataChanged,
                     this, &MainWindow::onTableDataChanged);
    QObject::connect(proxyModel, &SuicideSortFilterProxyModel::filterAccepted,
                     this, &MainWindow::onFilterAccepted);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::enableAllButtons(bool state)
{
    ui->DeselectAllButton->setEnabled(state);
    ui->DeleteRowButton->setEnabled(state);
    ui->FilterButton->setEnabled(state);
    ui->ResetFilterButton->setEnabled(state);
    ui->InsertRowButton->setEnabled(state);
    ui->RefreshDataButton->setEnabled(state);
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,
            tr("Open Data Storage File"), "", tr("CSV files (*.csv)"));

    if (filename.isNull())
        return;

    _model->openFile(filename);
    enableAllButtons();
}

void MainWindow::on_actionSave_triggered()
{
    _model->saveFile();
}

void MainWindow::on_actionSave_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Suicide Save"), "",
        tr("CSV File (*.csv);;All Files (*)"));

    if (fileName.isEmpty())
        return;

    _model->saveToFile(fileName);
}

// CUSTOM SLOTS

void MainWindow::onTableDataChanged()
{
    // ui->label->setText(QString::number(_model->getRowNum()) + " Items Shown");
    ui->label->setText(QString::number(proxyModel->rowCount()) + " Items Shown");
}

void MainWindow::onFilterAccepted()
{
    ui->InsertRowButton->setEnabled(false);
    ui->RefreshDataButton->setEnabled(false);
    onTableDataChanged();
}

void MainWindow::on_DeleteRowButton_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (select->hasSelection()) {
        QList<QModelIndex> selection = select->selectedRows();
        // selection.first().row();
        for (int i = 0; i < selection.count(); ++i) {
            QModelIndex index = selection.at(i);
            _model->removeRows(index.row() - i, 1, selection.first());
            onTableDataChanged();
        }
    }
}

void MainWindow::on_InsertRowButton_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (select->hasSelection()) {
        QList<QModelIndex> selection = select->selectedRows();
        for (int i = 0; i < selection.count(); ++i) {
            QModelIndex index = selection.at(i);
            _model->insertRows(index.row() + i + 1, 1, selection.first());
            onTableDataChanged();
        }
        return;
    }
    _model->insertRows(0, 1, QModelIndex());
}

void MainWindow::on_RefreshDataButton_clicked()
{
    _model->refreshFile();
}

void MainWindow::on_DeselectAllButton_clicked()
{
    ui->tableView->clearSelection();
}


void MainWindow::on_FilterButton_clicked()
{
//    filterView->show();
//    FilterDialog filterDialog;
//    filterDialog.setModal(true);
//    filterDialog.exec();
    filterView = new FilterDialog(proxyModel, this);
    filterView->show();
}

void MainWindow::on_ResetFilterButton_clicked()
{
    proxyModel->disableFilter();
    ui->InsertRowButton->setEnabled(true);
    ui->RefreshDataButton->setEnabled(true);
}












