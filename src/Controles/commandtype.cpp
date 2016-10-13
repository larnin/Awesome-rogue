#include "commandtype.h"
#include "Utilities/tr.h"
#include <cassert>

std::string commandTypeToString(CommandType type)
{
    switch (type)
    {
    case MOVE_UP:
        return tr("Up");
    case MOVE_DOWN:
        return tr("Down");
    case MOVE_LEFT:
        return tr("Left");
    case MOVE_RIGHT:
        return tr("Right");
    case SHOOT_UP:
        return tr("Fire up");
    case SHOOT_DOWN:
        return tr("Fire down");
    case SHOOT_LEFT:
        return tr("Fire left");
    case SHOOT_RIGHT:
        return tr("Fire right");
    case KEY_VALID:
        return tr("Accept");
    case KEY_CANCEL:
        return tr("Cancel");
    case KEY_PAUSE:
        return tr("Pause");
    case KEY_ACTION:
        return tr("Action");
    case KEY_MAP:
        return tr("Map");
    case KEY_INVENTARY:
        return tr("Inventory");
    case KEY_LEFT_PAGE:
        return tr("Left page");
    case KEY_RIGHT_PAGE:
        return tr("Right page");
    default:
        assert(false);
    break;
    }

    return "";
}
