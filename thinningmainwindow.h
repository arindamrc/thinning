#ifndef THINNINGMAINWINDOW_H
#define THINNINGMAINWINDOW_H

#include <QMainWindow>

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
};
#endif // THINNINGMAINWINDOW_H
