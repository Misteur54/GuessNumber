#include "JsonFormat.hpp"

JsonFormatByte::JsonFormatByte()
{

}

QByteArray JsonFormatByte::serialization(QJsonObject message)
{
    QJsonDocument doc(message);
    QByteArray bytes = doc.toJson();

    return (bytes);
}

QJsonObject JsonFormatByte::deserialization(QByteArray message)
{
    QJsonDocument doc = QJsonDocument::fromJson(message);
    QJsonObject test = doc.object();

    return (test);
}
