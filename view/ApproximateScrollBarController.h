#ifndef APPROXIMATESCROLLBARCONTROLLER_H
#define APPROXIMATESCROLLBARCONTROLLER_H

#include "ScrollBarStrategy.h"

class ApproximateScrollBarController : public ScrollBarStrategy {
public:
    using ScrollBarStrategy::ScrollBarStrategy;

    void onFileChanged();
};

#endif // APPROXIMATESCROLLBARCONTROLLER_H
