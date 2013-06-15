#ifndef APPROXIMATESCROLLBARCONTROLLER_H
#define APPROXIMATESCROLLBARCONTROLLER_H

#include "ScrollBarStrategy.h"
#include "ScreenPosition.h"

class ApproximateScrollBarController : public ScrollBarStrategy {
public:
    using ScrollBarStrategy::ScrollBarStrategy;

    void onFileChanged() override;
    ScreenPosition position() override;
private:
    int m_lastSBPosition = -1;
    ScreenPosition m_currentScreenPos;
};

#endif // APPROXIMATESCROLLBARCONTROLLER_H
