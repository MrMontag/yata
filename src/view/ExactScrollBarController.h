#ifndef EXACTSCROLLBARCONTROLLER_H
#define EXACTSCROLLBARCONTROLLER_H

#include "ScrollBarStrategy.h"
#include <vector>
#include <QScopedPointer>

class QScrollBar;
class YTextDocument;

class ExactScrollBarController : public ScrollBarStrategy
{
public:
    ExactScrollBarController(TailView *tailView);

    void onFileChanged();
    ScreenPosition position();
private:
    QScopedPointer<YTextDocument> m_document;
};

#endif // EXACTSCROLLBARCONTROLLER_H
