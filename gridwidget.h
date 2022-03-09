#ifndef GRIDWIDGET_H
#define GRIDWIDGET_H

#include <QWidget>

#include "globals.h"
#include "ArrayND.hpp"

struct Size {
    unsigned_t x, y;
};

class GridWidget : public QWidget
{
    Q_OBJECT
public:

    explicit GridWidget(QWidget *parent = nullptr);

protected:

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *_) override;

private:

    unsigned_t W, H;

    bool inSelectionState;

    ArrayND<uchar,2> grid;

    void updateCells(QMouseEvent *event);

    Size getSize() const;

    void drawBackground(QPainter* painter) const;
    void drawGridLines(QPainter* painter) const;
    void drawCells(QPainter* painter) const;

signals:

};

#endif // GRIDWIDGET_H
