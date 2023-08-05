#ifndef CAMERAPARAMETERSMODEL_H
#define CAMERAPARAMETERSMODEL_H


#include <QAbstractListModel>

class CameraParametersModel: public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool isPositionView  WRITE setIsPositionView);

public:
    explicit CameraParametersModel(QObject* parent = nullptr);
    ~CameraParametersModel();

public:
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    Q_INVOKABLE QVariantMap getRowData(int row) const;
    virtual QHash<int, QByteArray> roleNames() const override;

private:
    QList<QVariantMap> m_entries;
    void setIsPositionView(bool isPositionView);
};

#endif //CAMERAPARAMETERSMODEL_H
