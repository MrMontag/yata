#ifndef APPROXIMATESCROLLBARCONTROLLER_H
#define APPROXIMATESCROLLBARCONTROLLER_H

#include "ScrollBarStrategy.h"
#include "ScreenPosition.h"

class ApproximateScrollBarController : public ScrollBarStrategy {
public:
    ApproximateScrollBarController(TailView * view);

    void onFileChanged();
    ScreenPosition position();
private:
    int m_lastSBPosition;
    ScreenPosition m_currentScreenPos;
};

#endif // APPROXIMATESCROLLBARCONTROLLER_H
