#ifndef TEETHPARAMETERS_H
#define TEETHPARAMETERS_H

#include <QObject>
#include <QPointF>

class TeethParameters : public QObject
{
public:
    explicit TeethParameters(QObject* parent = nullptr);

    QVariantList getTeethsData();
    QVariantList getMandibularParameters();

private:
    QVariantMap getNumberInfo(int index);
    QVariantMap getTeethInfo(int index);

    QVector<QPointF> m_teethPoints;
    QVector<QPointF> m_numberPoints;

};




#endif // TEETHPARAMETERS_H
