#include "sqltablemodelalternativebackground.h"

#include <QColor>
#include "main.h"

static inline bool isNumeric(const QVariant &value)
{
    return value.type() == QVariant::Int || value.type() == QVariant::Double;
}

SqlTableModelAlternativeBackground::SqlTableModelAlternativeBackground(QObject *parent, QSqlDatabase db) :
    QSqlTableModel(parent, db), m_editable(true), m_view(false)
{
}

QVariant SqlTableModelAlternativeBackground::data(const QModelIndex & index, int role) const
{
    switch (role) {
    case Qt::BackgroundRole:
        if (isDirty(index)) {
            if (index.row() %2)
                return QVariant(QColor("#FFF4CF"));
            else
                return QVariant(QColor("#FFFAE9"));
        }
        if (index.row() % 2)
            return QVariant(QColor("#E8EDF5"));
        break;
    case Qt::DisplayRole: {
        QVariant d = QSqlTableModel::data(index, role);
        if (d.type() == QVariant::String) {
            return d.toString().split("\n").at(0);
        } else if (m_roundingPrecisionMap.contains(index.column()) && isNumeric(d)) {
            return QString::number(d.toDouble(), 'f', m_roundingPrecisionMap[index.column()]);
        }

        return d;
    }
    case Qt::TextAlignmentRole: {
        QVariant d = QSqlTableModel::data(index, Qt::DisplayRole);
        if (isNumeric(d))
            return (int)(Qt::AlignRight | Qt::AlignVCenter);

        return QSqlTableModel::data(index, role);
    }
    }

    return QSqlTableModel::data(index, role);
}

Qt::ItemFlags SqlTableModelAlternativeBackground::flags(const QModelIndex &index) const
{
    Qt::ItemFlags original = QSqlTableModel::flags(index);
    if (!m_editable || m_view) {
        return original & ~(Qt::ItemIsEditable);
    }
    return original;
}

void SqlTableModelAlternativeBackground::setTable(const QString &tableName)
{
    beginResetModel();
    clear();
    if (database().driverName() == "QSQLITE") {
        QSqlTableModel::setTable(addQuote(tableName));
    } else {
        QSqlTableModel::setTable(tableName);
    }

    m_view = false;
    foreach(QString view, database().tables(QSql::Views)) {
        if (view == plainTableName()) {
            m_view = true;
            break;
        }
    }
    endResetModel();
}

QString SqlTableModelAlternativeBackground::plainTableName() const
{
    return removeQuote(QSqlTableModel::tableName());
}

long long SqlTableModelAlternativeBackground::sqlRowCount()
{
    QSqlQuery count;
    if (filter().isEmpty()) {
        count = database().exec(QString("SELECT count(*) FROM %1;").arg(tableName()));
    } else {
        count = database().exec(QString("SELECT count(*) FROM %1 WHERE %2;").arg(tableName(), filter()));
    }

    count.next();
    return count.value(0).toLongLong();
}

bool SqlTableModelAlternativeBackground::isNumericColumn(int column) const
{
    QVariant d = SqlTableModelAlternativeBackground::data(createIndex(0, column), Qt::DisplayRole);
    return isNumeric(d);
}

int SqlTableModelAlternativeBackground::roundingPrecision(int column) const
{
    return m_roundingPrecisionMap.contains(column) ? m_roundingPrecisionMap[column] : -1;
}

void SqlTableModelAlternativeBackground::setRoundingPrecision(int column, int precision)
{
    if (column < 0 || columnCount() <= column) return;

    if (precision >= 0) {
        m_roundingPrecisionMap[column] = precision;
        emit roundingPrecisionChanged(column);
    } else {
        m_roundingPrecisionMap.remove(column);
        emit roundingPrecisionChanged(column);
    }
}
