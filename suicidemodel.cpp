#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDir>
#include <QSortFilterProxyModel>
#include "suicidemodel.h"

//Suicides::Suicides(SuicideModel* model, QObject* parent)
//    : QObject(parent)
//    , _model(model)
//{

//}

//Suicide Suicides::getSuicide(const int& row) {
//    return _data[row];
//}

SuicideModel::SuicideModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int SuicideModel::rowCount(const QModelIndex & /*parent*/) const
{
   return _suicides.size();
}

int SuicideModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 7;
}

QVariant SuicideModel::headerData(int section, Qt::Orientation orientation, int role) const
 {
     if (role != Qt::DisplayRole)
         return QVariant();

     if (orientation == Qt::Vertical)
           return QString::number(section);

     if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0: return tr("State");
            case 1: return tr("Year");
            case 2: return tr("Type code");
            case 3: return tr("Type");
            case 4: return tr("Gender");
            case 5: return tr("Age group");
            case 6: return tr("Total");
            default: return QVariant();
         }
     }
     return QVariant();
 }

QVariant SuicideModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int column = index.column();

    if (role == Qt::DisplayRole)
        return _suicides[row].getValue(column);
//       return QString("Row%1, Column%2")
//                   .arg(row + 1)
//                   .arg(column +1);

    return QVariant();
}

bool SuicideModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (role == Qt::EditRole)
    {
        QString result = value.toString();

        if (result == "")
            return false;

        // beginResetModel();
        switch(index.column()) {
            case 0: _suicides[index.row()].state = result; break;
            case 1: _suicides[index.row()].year = result.toInt(); break;
            case 2: _suicides[index.row()].typeCode = result; break;
            case 3: _suicides[index.row()].type = result; break;
            case 4: _suicides[index.row()].gender = result; break;
            case 5: _suicides[index.row()].ageGroup = result; break;
            case 6: _suicides[index.row()].total = result.toInt(); break;
            default: ;
        }
        emit editCompleted( result );
    }
    return true;
}

Qt::ItemFlags SuicideModel::flags(const QModelIndex & /*index*/) const
{
    return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;
}

bool SuicideModel::insertRows(int position, int count, const QModelIndex &parent)
{
    beginInsertRows(QModelIndex(), position, position + count - 1);
    for (int i = position; i < position + count; ++i) {
        _suicides.insert(i, Suicide());
    }
    endInsertRows();
    return true;
}

bool SuicideModel::removeRows(int position, int count, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), position, position + count);
    for (int i = position; i < position + count; ++i) {
        _suicides.removeAt(i);
    }
    endRemoveRows();
    return true;
}

// MODEL FUNCTIONS

void SuicideModel::openFile(const QString& fileName)
{
    _filename = fileName;
    _suicides.clear();
    if (readFile())     
        emit tableDataChanged();
}

bool SuicideModel::readFile()
{
    if (_filename.isEmpty())
        throw std::invalid_argument("File name not set");

    QFile file(_filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox box;
        box.setText("Can't open file to read a model!");
        box.exec();

        return false;
    }

    QTextStream fStream(&file);
    bool skipLine = true;

    beginResetModel();

    while (!fStream.atEnd())
    {
        QString line = fStream.readLine();
        if (skipLine) {
            skipLine = false; continue;
        }
        QStringList lineData = line.split( "," );
        Suicide suicide = { lineData[0], lineData[1].toInt(), lineData[2], lineData[3], lineData[4], lineData[5], lineData[6].toInt() };
        _suicides.append(suicide);
    }

    endResetModel();

    file.close();

    return true;
}

void SuicideModel::saveToFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly|QFile::Truncate)) {
        QMessageBox box;
        box.setText("Can't save file!");
        box.exec();

        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << "State,Year,Type_code,Type,Gender,Age_group,Total\n";

    for (int i = 0; i < _suicides.size(); ++i) {
        QString line = _suicides[i].getCSVRow();
        out << line << "\n";
    }

    file.close();
    return;
}

void SuicideModel::saveFile()
{
    saveToFile(_filename);
}

void SuicideModel::refreshFile()
{
    openFile(_filename);
}


// FILTER MODEL

SuicideSortFilterProxyModel::SuicideSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{

}

void SuicideSortFilterProxyModel::setFilter(const FilterRequest request)
{
    _request = request;
    invalidateFilter();
    emit filterAccepted();
}

bool SuicideSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (!_request.enabled) return true;
    bool flag = true;
    for(int i = 0; i < sourceModel()->columnCount(); ++i)
    {
        QModelIndex index = sourceModel()->index(sourceRow, i, sourceParent);
        QString source = sourceModel()->data(index).toString().toLower();
        QString s = _request.getValue(i).toLower();
        if (i == 1) {
            if (s == "0") { continue; }
            if (s == source) { continue; }
            return false;
        }
        if (i == 4) {
            if (s == "any") { continue; }
            if ((s == "male") && (source == "male")) { continue; }
            if ((s == "female") && (source == "female")) { continue; }
            return false;
        }
        if (i == 5) {
            if (s == "any") { continue; }
        }
        if (s != "") {
            if (!source.contains(s)) { flag = false; break; }
        }
    }
    return flag;
}

void SuicideSortFilterProxyModel::disableFilter()
{
    _request.enabled = false;
    invalidateFilter();
    emit filterAccepted();
}






