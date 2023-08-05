#include "TeethModel.h"
#include "TeethParameters.h"

#include "IconHelper.h"

#include <QColor>

TeethModel::TeethModel(QObject *parent)
    : QAbstractListModel{parent}
{
    TeethParameters params;
    QVariantList data = params.getTeethsData();

    for(auto& item : data){
        QVariantMap map = item.toMap();
        m_entries << map;
    }
}

TeethModel::~TeethModel()
{
}
QColor getColorFromState(const TeethState& state){
    switch (state) {
    case TeethState::Normal:
        return QColor::fromString("#2D2F44");
    case TeethState::Deciduous:
        return QColor::fromString("#ff0000");
    case TeethState::Remove:
    case TeethState::Missing:
        return QColor::fromString("#d7d8dc");
    }

}
void TeethModel::setState(int id, const TeethState& state)
{
    for(int i = 0; i < m_entries.size(); ++i){
        if(m_entries[i]["index"].toInt() == id){
            TeethState oldState = static_cast<TeethState>(m_entries[i]["state"].toInt());
            if(oldState == state)
                return;

            m_entries[i]["state"] = static_cast<int>(state);

            QVariantMap map = m_entries[i]["teethInfo"].toMap();
            map["icon"] = IconHelper::getExternalIconWithColor(m_entries[i]["teethInfo"].toMap()["icon"].toString(), getColorFromState(oldState), getColorFromState(state));
            m_entries[i]["teethInfo"] = map;

            emit dataChanged(index(i, 0), index(i, 0));
            return;
        }
    }
}

void TeethModel::setTeethNumber(int id, const TeethNumberingSystem& number)
{
    for(int i = 0; i < m_entries.size(); ++i){
        if(m_entries[i]["index"].toInt() == id){

            switch(number)
            {
            case TeethNumberingSystem::X:
                break;
            case TeethNumberingSystem::UniversalNumberingSystem:
                m_entries[i]["number"] = m_entries[i]["numberInfo"].toMap()["menu"].toStringList()[0];
                break;
            case TeethNumberingSystem::FDI:
            {
                m_entries[i]["number"] = m_entries[i]["numberInfo"].toMap()["menu"].toStringList()[2];
                break;
            }
            case TeethNumberingSystem::PalmerNotation:
                m_entries[i]["number"] = m_entries[i]["numberInfo"].toMap()["menu"].toStringList()[1];
                break;
            }
            emit dataChanged(index(i, 0), index(i, 0));
            break;
        }
    }
}

void TeethModel::resetStates(bool isMaxillary)
{
    int start = isMaxillary ? 0 :  16;
    int end = isMaxillary ? 16 : 32;
    for(int i = start; i <= end; ++i){
        setState(i, TeethState::Normal);
    }
}

int TeethModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_entries.size();
}

QVariant TeethModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::UserRole)
        return QVariant::fromValue(m_entries[index.row()]);

    return QVariant();
}

QHash<int, QByteArray> TeethModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::UserRole] = "$parameters";
    return roles;
}
