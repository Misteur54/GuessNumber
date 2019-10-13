#ifndef JSONFORMAT_HPP
#define JSONFORMAT_HPP

#include <QtCore>

class JsonFormatByte
{
public:
    JsonFormatByte();
    QByteArray serialization(QJsonObject message);
    QJsonObject deserialization(QByteArray message);
};

#endif // JSONFORMAT_HPP
