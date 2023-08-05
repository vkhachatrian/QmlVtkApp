#include "CameraParametersModel.h"
#include "Parameters.h"

#include <QVariantMap>
#include <QUrl>

CameraParametersModel::CameraParametersModel(QObject *parent)
    :QAbstractListModel(parent){}

CameraParametersModel::~CameraParametersModel()
{}

int CameraParametersModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_entries.size();
}

QVariant CameraParametersModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

     if(role == Qt::UserRole)
        return QVariant::fromValue(m_entries[index.row()]);

    return QVariant();
}

QVariantMap CameraParametersModel::getRowData(int row) const
{
    return m_entries[row];
}

QHash<int, QByteArray> CameraParametersModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::UserRole] = "$parameters";
    return roles;
}

void CameraParametersModel::setIsPositionView(bool isPositionView)
{
    if(isPositionView){
        m_entries << Parameters::instance()->getViewButtonsParamsType(ViewButtonsParamsType::eFront);
        m_entries << Parameters::instance()->getViewButtonsParamsType(ViewButtonsParamsType::eBack);
        m_entries << Parameters::instance()->getViewButtonsParamsType(ViewButtonsParamsType::eRight);
        m_entries << Parameters::instance()->getViewButtonsParamsType(ViewButtonsParamsType::eLeft);
        m_entries << Parameters::instance()->getViewButtonsParamsType(ViewButtonsParamsType::eTop);
        m_entries << Parameters::instance()->getViewButtonsParamsType(ViewButtonsParamsType::eBottom);
    }
    else {
        m_entries << Parameters::instance()->getCameraModeParamsType(CameraModeParamsType::eFreeRotation);
        m_entries << Parameters::instance()->getCameraModeParamsType(CameraModeParamsType::eXAxis);
        m_entries << Parameters::instance()->getCameraModeParamsType(CameraModeParamsType::eYAxis);
        m_entries << Parameters::instance()->getCameraModeParamsType(CameraModeParamsType::eZAxis);
        m_entries << Parameters::instance()->getCameraModeParamsType(CameraModeParamsType::ePanView);
        m_entries << Parameters::instance()->getCameraModeParamsType(CameraModeParamsType::eZoomView);
    }
}
