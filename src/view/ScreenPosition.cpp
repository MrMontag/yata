#include "ScreenPosition.h"

ScreenPosition::ScreenPosition(qint64 a, int bl):
    address(a),
    blockLine(bl)
{
}

bool ScreenPosition::operator==(const ScreenPosition & other) const
{
    return address == other.address && blockLine == other.blockLine;
}

const ScreenPosition ScreenPosition::MAX = ScreenPosition(-2, -2);
