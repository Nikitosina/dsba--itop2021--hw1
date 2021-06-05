#ifndef SUICIDEMODEL_H
#define SUICIDEMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QSortFilterProxyModel>

//int COLUMNS = 0;
//int ROWS = 0;

//enum Gender {
//    male = "Male", female = "Female";
//};

struct Suicide
{
    QString state;
    int year;
    QString typeCode;
    QString type;
    QString gender;
    QString ageGroup;
    int total;

    QString getValue(int index) const
    {
        if (index == 0) { return state; }
        if (index == 1) { return QString::number(year); }
        if (index == 2) { return typeCode; }
        if (index == 3) { return type; }
        if (index == 4) { return gender; }
        if (index == 5) { return ageGroup; }
        if (index == 6) { return QString::number(total); }
        return "";
    }

    QString getCSVRow() const
    {
        return state + "," + QString::number(year) + "," + typeCode + "," + type + "," + gender + "," + ageGroup + "," + QString::number(total);
    }
};

// stores substrings
struct FilterRequest
{
    QString state;
    QString year;
    QString typeCode;
    QString type;
    QString gender;
    QString ageGroup;
    QString total;
    bool enabled = true;

    void resetAll()
    {
        state = "";
        year = "";
        typeCode = "";
        type = "";
        gender = "";
        ageGroup = "";
        total = "";
    }

    QString getValue(int index) const
    {
        if (index == 0) { return state; }
        if (index == 1) { return year; }
        if (index == 2) { return typeCode; }
        if (index == 3) { return type; }
        if (index == 4) { return gender; }
        if (index == 5) { return ageGroup; }
        if (index == 6) { return total; }
        return "";
    }
};

class SuicideModel : public QAbstractTableModel
{
    Q_OBJECT

private:
    // QVariant tableData[ROWS][COLUMNS];
    QList<Suicide> _suicides;
    // QString m_gridData[ROWS][COLS];

public:
    explicit SuicideModel(QObject *parent = nullptr);
    // required methods for QAbstractModel
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex & index) const override;
    bool insertRows(int position, int count, const QModelIndex &parent) override;
    bool removeRows(int position, int count, const QModelIndex &parent) override;
    // bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;
    // bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    void openFile(const QString& fileName);
    void saveToFile(const QString& fileName);
    void refreshFile();
    void saveFile();
    int getRowNum() { return _suicides.size(); };

protected:
    QString _filename;

    bool readFile();
signals:
    void tableDataChanged();
    void editCompleted(const QString &);
};

class SuicideSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit SuicideSortFilterProxyModel(QObject *parent = 0);

    void setFilter(const FilterRequest request);
    void disableFilter();
    // void removeFilter(QString value);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    FilterRequest _request;
signals:
    void filterAccepted();
};

//class Suicides: public QObject
//{
//    Q_OBJECT

//public:
//    explicit Suicides(SuicideModel* model, QObject* parent = nullptr);
//    Suicide getSuicide(const int& row);
//protected:
//    SuicideModel* _model;
//private:
//    QList<Suicide> _data;
//};

#endif // SUICIDEMODEL_H
