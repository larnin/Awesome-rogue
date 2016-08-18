#include "commandtype.h"
#include <cassert>

std::string commandTypeToString(CommandType type)
{
    switch (type)
    {
    case MOVE_UP:
        return "Haut";
    case MOVE_DOWN:
        return "Bas";
    case MOVE_LEFT:
        return "Gauche";
    case MOVE_RIGHT:
        return "Droite";
    case SHOOT_UP:
        return "Tir haut";
    case SHOOT_DOWN:
        return "Tir bas";
    case SHOOT_LEFT:
        return "Tir gauche";
    case SHOOT_RIGHT:
        return "Tir droite";
    case KEY_VALID:
        return "Accepter";
    case KEY_CANCEL:
        return "Annuler";
    case KEY_PAUSE:
        return "Pause";
    case KEY_ACTION:
        return "Action";
    case KEY_MAP:
        return "Map";
    case KEY_INVENTARY:
        return "Inventaire";
    case KEY_LEFT_PAGE:
        return "Page gauche";
    case KEY_RIGHT_PAGE:
        return "Page droite";
    default:
        assert(false);
    break;
    }

    return "";
}
