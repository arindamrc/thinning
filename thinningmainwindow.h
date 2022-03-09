#ifndef THINNINGMAINWINDOW_H
#define THINNINGMAINWINDOW_H

#include <QMainWindow>

#include "config.h"
#include "gridwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ThinningMainWindow; }
QT_END_NAMESPACE

class ThinningMainWindow : public QMainWindow
{
    Q_OBJECT

public:

    ThinningMainWindow(QWidget *parent = nullptr);
    ~ThinningMainWindow();

private:
    Ui::ThinningMainWindow *ui;

    GridWidget* gridWidget;

    void createToolbar();
};
#endif // THINNINGMAINWINDOW_H
