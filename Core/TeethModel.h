#ifndef TEETHMODEL_H
#define TEETHMODEL_H

#include <QAbstractListModel>

#include "Enums.h"

class TeethModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TeethModel(QObject *parent = nullptr);
    ~TeethModel();

    Q_INVOKABLE void setState(int id, const TeethState& state);
    Q_INVOKABLE void setTeethNumber(int id, const TeethNumberingSystem& number);
    Q_INVOKABLE void resetStates(bool isMaxillary = false);

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

private:
    QVector<QVariantMap> m_entries;
};

#endif // TEETHMODEL_H
