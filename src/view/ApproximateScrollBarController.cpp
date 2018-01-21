#include "ApproximateScrollBarController.h"
#include "TailView.h"
#include "PartialLayout.h"

#include <QScrollBar>

const qint64 APPROXIMATE_CHARS_PER_LINE = 20;

ApproximateScrollBarController::ApproximateScrollBarController(TailView * view):
	ScrollBarStrategy(view),
	m_lastSBPosition(-1)
{
}
    
void ApproximateScrollBarController::onFileChanged()
{
}

ScreenPosition ApproximateScrollBarController::position()
{
    QScrollBar * verticalScrollBar = view()->verticalScrollBar();

    if (verticalScrollBar->sliderPosition() != m_lastSBPosition)
    {
        m_lastSBPosition = verticalScrollBar->sliderPosition();
        if(verticalScrollBar->sliderPosition() >= verticalScrollBar->maximum()) {
            m_currentScreenPos = ScreenPosition::MAX;
        } else {
            m_currentScreenPos.address = static_cast<qint64>(verticalScrollBar->sliderPosition()) * APPROXIMATE_CHARS_PER_LINE;
            m_currentScreenPos.blockLine = 0;
        }
    }

    return m_currentScreenPos;
}
