#ifndef PACKET_TYPE_H
#define PACKET_TYPE_H

enum class PacketType {
    creat_room = 1,
    guest_join,
    move,
    resign,
    chat,
    PauseRequest,
    PauseResponse
};

#endif // PACKET_TYPE_H
