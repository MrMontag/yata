#ifndef EXACTSCROLLBARCONTROLLER_H
#define EXACTSCROLLBARCONTROLLER_H

#include "ScrollBarStrategy.h"

class QScrollBar;

class ExactScrollBarController : public ScrollBarStrategy
{
public:
    using ScrollBarStrategy::ScrollBarStrategy;

    void onFileChanged();
};

#endif // EXACTSCROLLBARCONTROLLER_H
