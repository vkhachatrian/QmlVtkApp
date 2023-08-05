#include "TeethParameters.h"
#include "Enums.h"

#include <QVariantMap>

TeethParameters::TeethParameters(QObject *parent)
    : QObject(parent)
{
    m_teethPoints = { QPointF(47, 228), QPointF(50, 190), QPointF(57, 150),  QPointF(68, 122),  QPointF(80, 99),
                      QPointF(95, 77),  QPointF(112, 57), QPointF(134, 45),  QPointF(166, 45),  QPointF(195, 57),
                      QPointF(211, 77), QPointF(226, 99), QPointF(236, 122), QPointF(239, 150), QPointF(248, 190), QPointF(259, 228),

                      QPointF(253, 287), QPointF(243, 329), QPointF(230, 367), QPointF(222, 402), QPointF(213, 427), QPointF(201, 447), QPointF(182, 457), QPointF(164, 460),
                      QPointF(146, 460),  QPointF(128, 457), QPointF(107, 447), QPointF(98, 427), QPointF(83, 402), QPointF(66, 367),  QPointF(55, 329),   QPointF(43, 287)
                    };

    m_numberPoints = { QPointF(31.641, 235), QPointF(32.109, 200), QPointF(38.797, 163), QPointF(44.063, 130), QPointF(66.266, 95),
                       QPointF(86.313, 65),  QPointF(117.422, 40), QPointF(148.281, 25), QPointF(184, 25),     QPointF(216, 45),
                       QPointF(241, 65),     QPointF(261, 95),     QPointF(281, 130),    QPointF(289, 163),    QPointF(296, 200), QPointF(299, 235),

                       QPointF(303, 307),    QPointF(288, 357), QPointF(275, 397),     QPointF(258, 424),     QPointF(238, 450),     QPointF(220, 474),    QPointF(195, 494), QPointF(173, 502),
                       QPointF(152.375, 502), QPointF(130.422, 494),  QPointF(106.531, 474),QPointF(87.391, 450),  QPointF(67.594, 424), QPointF(50.047, 397), QPointF(40.453, 357), QPointF(22.922, 307)
                     };
}

QVariantList TeethParameters::getTeethsData()
{
    QVariantList data;
    data.resize(32);

    for(int i = 0; i < data.size(); ++i){
        QVariantMap map;
        map["numberInfo"] = getNumberInfo(i);
        map["teethInfo"] = getTeethInfo(i);
        map["index"] = i + 1;
        map["number"] = "X";
        map["state"] = static_cast<int>(TeethState::Normal);
        data[i] = map;
    }
    return data;
}

QVariantMap TeethParameters::getNumberInfo(int index)
{
    QVariantMap numberInfo;
    numberInfo["x"] = m_numberPoints[index].x();
    numberInfo["y"] = m_numberPoints[index].y();

    QStringList menuList = { QString::number(index + 1) };
    if(index <= 7)
        menuList << "UR" + QString::number((18 - index) % 10) << QString::number(18 - index);
    else if(index <= 15)
        menuList << "UL" + QString::number((13 + index) % 10) << QString::number(13 + index);
    else if(index <= 23)
        menuList << "LL" + QString::number((54 - index) % 10) << QString::number(54 - index);
    else
        menuList << "LR" + QString::number((17 + index) % 10) << QString::number(17 + index);

    numberInfo["menu"] =  menuList;

    return numberInfo;
}

QVariantMap TeethParameters::getTeethInfo(int index)
{
    QVariantMap teethInfo;
    teethInfo["x"] = m_teethPoints[index].x();
    teethInfo["y"] = m_teethPoints[index].y();
    teethInfo["menu"] = index < 3 || index > 28 || (index > 12 && index < 19) ? QStringList{"Remove", "Missing"} : QStringList{"Remove", "Missing", "Deciduous"};
    if(index <= 7)
        teethInfo["icon"] = "qrc:/Icons/Teeths/Tooth" + QString::number(18 - index);
    else if(index <= 15)
        teethInfo["icon"] = "qrc:/Icons/Teeths/Tooth" + QString::number(13 + index);
    else if(index <= 23)
        teethInfo["icon"] = "qrc:/Icons/Teeths/Tooth" + QString::number(54 - index);
    else
        teethInfo["icon"] = "qrc:/Icons/Teeths/Tooth" + QString::number(17 + index);

    return teethInfo;
}
