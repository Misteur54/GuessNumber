#ifndef IPACKET_HPP
#define IPACKET_HPP
#include  <QJsonObject>

class IPacket {
    public:
        virtual ~IPacket() = default;
        virtual QByteArray serialization(QJsonObject message) = 0;
        virtual QJsonObject deserialization(QByteArray message) = 0;
};

#endif // IPACKET_HPP
