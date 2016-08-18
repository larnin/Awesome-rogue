#ifndef COMMANDTYPE_H
#define COMMANDTYPE_H

#include <string>

enum CommandType
{
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    SHOOT_UP,
    SHOOT_DOWN,
    SHOOT_LEFT,
    SHOOT_RIGHT,
    KEY_VALID,
    KEY_CANCEL,
    KEY_PAUSE,
    KEY_ACTION,
    KEY_MAP,
    KEY_INVENTARY,
    KEY_LEFT_PAGE,
    KEY_RIGHT_PAGE,
    COMMANDS_COUNT
};

std::string commandTypeToString(CommandType type);


#endif // COMMANDTYPE_H
