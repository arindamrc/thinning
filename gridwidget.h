#ifndef GRIDWIDGET_H
#define GRIDWIDGET_H

#include <QWidget>
#include <opencv2/core.hpp>

#include "globals.h"
#include "algos.h"

struct Size {
    unsigned_t x, y;
};

class GridWidget : public QWidget
{
    Q_OBJECT

public:

    explicit GridWidget(QWidget *parent = nullptr);

    bool isUpdateEnabled() const;

protected:

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

private:

    bool enableUpdate_ = false;

    uchar pass = 0;

    unsigned_t W_, H_;

    cv::Mat grid_;

    cv::Mat output_;

    ParallelIterativeThinning thinner_;

    void updateCells(QMouseEvent *event);

    Size getSize() const;

    void drawBackground(QPainter* painter) const;
    void drawGridLines(QPainter* painter) const;
    void drawCells(QPainter* painter) const;

public slots:

    void clear();
    void iterate();
    void subIterate();
    void result();
    void toggleUpdate();
};

#endif // GRIDWIDGET_H
