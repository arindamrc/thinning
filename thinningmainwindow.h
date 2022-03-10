#ifndef THINNINGMAINWINDOW_H
#define THINNINGMAINWINDOW_H

#include <QMainWindow>
#include <QSignalMapper>

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

    QSignalMapper algorithmSelectionMapper;

    void createOperationsToolbar();
    void createAlgorithmsToolbar();
    void createStatusBar();

    QString getAlgoName(const int idx);

private slots:

    void selectAlgorithm(int idx);

};
#endif // THINNINGMAINWINDOW_H
