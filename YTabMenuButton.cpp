#include "YTabMenuButton.h"
#include <QToolButton>
#include <QMenu>
#include <QPainter>
#include <QStyleOptionToolButton>

YTabMenuButton::YTabMenuButton(QWidget *parent)
    : QToolButton(parent)
{
    setArrowType(Qt::DownArrow);
    setPopupMode(QToolButton::InstantPopup);
}

YTabMenuButton::~YTabMenuButton()
{
}

void YTabMenuButton::paintEvent(QPaintEvent *)
{
    QStyleOptionToolButton option;
    initStyleOption(&option);
    option.features &= ~QStyleOptionToolButton::HasMenu;

    QPainter painter(this);
    style()->drawComplexControl(QStyle::CC_ToolButton, &option, &painter, this);
}
