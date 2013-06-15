#ifndef SCREENPOSITION_H
#define SCREENPOSITION_H

#include <QtGlobal>

struct ScreenPosition {
    qint64 address = 0;
    int blockLine = 0;

    ScreenPosition(qint64 a=0, int bl=0);
    bool operator==(const ScreenPosition & other) const;

    const static ScreenPosition MAX;
};

#endif // SCREENPOSITION_H
