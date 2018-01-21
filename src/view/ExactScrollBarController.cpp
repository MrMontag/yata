#include "ExactScrollBarController.h"
#include "fileio/FileBlockReader.h"
#include "view/PartialLayout.h"
#include "view/TailView.h"
#include "document/YTextDocument.h"

#include <QScrollBar>

ExactScrollBarController::ExactScrollBarController(TailView *tailView):
    ScrollBarStrategy(tailView),
    m_document(new YTextDocument())
{
}

void ExactScrollBarController::onFileChanged()
{
    QString data;
    std::vector<qint64> lineAddresses;
    FileBlockReader blockReader(view()->filename());
    /*bool success =*/ blockReader.readAll(&data, &lineAddresses);
    m_document->setText(data, lineAddresses);
    m_document->layout(view()->width());
    view()->updateScrollBars(m_document->numLayoutLines() - view()->numLinesOnScreen());
    //if(view()->followTail()) {
    //    scrollBar()->setValue(scrollBar()->maximum());
    //}

    //if(!success) { *error = blockReader.errorString(); }

    //return success;
}

ScreenPosition ExactScrollBarController::position()
{
    int sbpos = scrollBar()->sliderPosition();
    int topOfBlock = 0;
    QTextBlock block = m_document->blockLayoutLines().findContainingBlock(sbpos, &topOfBlock);
    qint64 addr = m_document->blockAddress(block);
    ScreenPosition pos(addr, sbpos - topOfBlock);
    return pos;
}
