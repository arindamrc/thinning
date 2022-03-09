#include "gridwidget.h"
#include "config.h"

#include <QMouseEvent>
#include <QPainter>

GridWidget::GridWidget(QWidget *parent) : QWidget(parent)
{
    W = Config::gridWidth;
    H = Config::gridHeight;
    grid.updateDimSizes(H,W);
    inSelectionState = false;
}

void GridWidget::mousePressEvent(QMouseEvent *event)
{
    updateCells(event);
    QWidget::mousePressEvent(event);
    update();
}

void GridWidget::mouseMoveEvent(QMouseEvent *event)
{
    updateCells(event);
    QWidget::mouseMoveEvent(event);
    update();
}

void GridWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    drawBackground(&painter);
    drawGridLines(&painter);
    drawCells(&painter);
}

void GridWidget::updateCells(QMouseEvent *event)
{
    bool deselect = event->modifiers() & Qt::ControlModifier;
    Size sz = getSize();
    unsigned_t cellWidth = sz.x, cellHeight = sz.y;
    unsigned_t col = event->x() / cellWidth;
    unsigned_t row = event->y() / cellHeight;
    grid(row, col) = !deselect;
}

Size GridWidget::getSize() const
{
    Size sz;
    sz.x = width() / W;
    sz.y = height() / H;
    return sz;
}

void GridWidget::drawBackground(QPainter *painter) const
{
    QBrush brush;
    brush.setColor(Qt::white);
    brush.setStyle(Qt::SolidPattern);

    painter->save();
    painter->drawRect(0, 0, width(), height());
    painter->restore();
}

void GridWidget::drawGridLines(QPainter *painter) const
{
    Size sz = getSize();
    unsigned_t cellWidth = sz.x, cellHeight = sz.y;

    QBrush brush;
    brush.setColor(Qt::black);
    brush.setStyle(Qt::SolidPattern);

    painter->save();
    painter->setBrush(brush);

    // Draw horizontal lines.
    for (int r = 0; r < height(); r += cellHeight) {
        painter->drawLine(0, r, width(), r);
    }

    // Draw vertical lines.
    for (int c = 0; c < width(); c += cellWidth) {
        painter->drawLine(c, 0, c, height());
    }

    painter->restore();
}

void GridWidget::drawCells(QPainter *painter) const
{
    Size sz = getSize();
    unsigned_t cellWidth = sz.x, cellHeight = sz.y;

    QBrush brush;
    brush.setColor(Qt::red);
    brush.setStyle(Qt::SolidPattern);

    painter->save();
    painter->setBrush(brush);
    for (unsigned_t r = 0; r < H; r++) {
        for (unsigned_t c = 0; c < W; c++) {
            if (grid(r,c)) {
                unsigned_t x = c * cellWidth;
                unsigned_t y = r * cellHeight;
                painter->drawRect(x, y, cellWidth, cellHeight);
            }
        }
    }
    painter->restore();
}
