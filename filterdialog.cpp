#include "filterdialog.h"
#include "ui_filterdialog.h"

FilterDialog::FilterDialog(SuicideSortFilterProxyModel *filterModel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDialog)
{
    ui->setupUi(this);
    _filterModel = filterModel;
    ui->genderComboBox->addItem("Any");
    ui->genderComboBox->addItem("Male");
    ui->genderComboBox->addItem("Female");
    ui->genderComboBox->setCurrentIndex(0);

    ui->ageGroupComboBox->addItem("Any");
    ui->ageGroupComboBox->addItem("0-14");
    ui->ageGroupComboBox->addItem("15-29");
    ui->ageGroupComboBox->addItem("30-44");
    ui->ageGroupComboBox->addItem("45-59");
    ui->ageGroupComboBox->addItem("60+");
    ui->ageGroupComboBox->addItem("0-100+");
    ui->genderComboBox->setCurrentIndex(0);

    ui->yearSpinBox->setValue(0);
}

FilterDialog::~FilterDialog()
{
    delete ui;
}

void FilterDialog::on_buttonBox_accepted()
{
    _request.resetAll();
    _request.state = ui->stateLineEdit->text();
    _request.year = ui->yearSpinBox->text();
    _request.typeCode = ui->typeCodeLineEdit->text();
    _request.type = ui->typeLineEdit->text();
    _request.gender = ui->genderComboBox->itemText(ui->genderComboBox->currentIndex());
    _request.ageGroup = ui->ageGroupComboBox->itemText(ui->ageGroupComboBox->currentIndex());
    _request.total = ui->totalLineEdit->text();
    _request.enabled = true;

    _filterModel->setFilter(_request);
}
