#include "gridwidget.h"
#include "config.h"

#include <QMouseEvent>
#include <QPainter>

GridWidget::GridWidget(QWidget *parent) : QWidget(parent)
{
    enableUpdate_ = false;
    W_ = Config::gridWidth;
    H_ = Config::gridHeight;
    grid_ = cv::Mat::zeros(cv::Size(W_, H_), cv::DataType<uchar>::type);
    output_ = cv::Mat::zeros(cv::Size(W_, H_), cv::DataType<uchar>::type);
}

bool GridWidget::isUpdateEnabled() const
{
    return enableUpdate_;
}

void GridWidget::mousePressEvent(QMouseEvent *event)
{
    if (enableUpdate_) {
        updateCells(event);
    }
    QWidget::mousePressEvent(event);
    update();
}

void GridWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (enableUpdate_) {
        updateCells(event);
    }
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
    grid_.at<uchar>(row, col) = !deselect;
}

Size GridWidget::getSize() const
{
    Size sz;
    sz.x = width() / W_;
    sz.y = height() / H_;
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
    for (unsigned_t r = 0; r < H_; r++) {
        for (unsigned_t c = 0; c < W_; c++) {
            if (grid_.at<uchar>(r,c)) {
                unsigned_t x = c * cellWidth;
                unsigned_t y = r * cellHeight;
                painter->drawRect(x, y, cellWidth, cellHeight);
            }
        }
    }
    painter->restore();
}

void GridWidget::clear()
{
    grid_.setTo(0);
    update();
}

void GridWidget::iterate()
{
    update();
}

void GridWidget::subIterate()
{
    update();
}

void GridWidget::result()
{
    output_.setTo(0);
    thinner_.compute(grid_, output_, ParallelIterativeThinning::ZHANG_SUEN_NWSE);
    grid_ = output_.clone();
    update();
}

void GridWidget::toggleUpdate()
{
    enableUpdate_ = !enableUpdate_;
}
