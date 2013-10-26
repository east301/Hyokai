#ifndef SQLTABLEMODELALTERNATIVEBACKGROUND_H
#define SQLTABLEMODELALTERNATIVEBACKGROUND_H

#include <QSqlTableModel>
#include <QMap>

class SqlTableModelAlternativeBackground : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit SqlTableModelAlternativeBackground(QObject *parent = 0, QSqlDatabase db = QSqlDatabase() );
    
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;

    virtual void setTable(const QString &tableName);
    QString plainTableName() const;

    bool editable() { return m_editable && !m_view; }
    bool isView() { return m_view; }
    long long sqlRowCount();

    bool isNumericColumn(int column) const;
    int roundingPrecision(int column) const;
    void setRoundingPrecision(int column, int precision);

private:
    bool m_editable;
    bool m_view;
    QMap<int, int> m_roundingPrecisionMap;

signals:
    void roundingPrecisionChanged(int column);

public slots:
    void setEditable(bool editable) {m_editable = editable;}
};

#endif // SQLTABLEMODELALTERNATIVEBACKGROUND_H
