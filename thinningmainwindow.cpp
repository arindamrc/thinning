#include "thinningmainwindow.h"
#include "ui_thinningmainwindow.h"

#include <QToolBar>

ThinningMainWindow::ThinningMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ThinningMainWindow)
{
    ui->setupUi(this);
    setWindowTitle(Config::winTitle);
    setMinimumHeight(Config::winHeight);
    setMinimumWidth(Config::winWidth);
    gridWidget = new GridWidget(this);
    setCentralWidget(gridWidget);
    createOperationsToolbar();
    createAlgorithmsToolbar();
    createStatusBar();
}

ThinningMainWindow::~ThinningMainWindow()
{
    delete gridWidget;
    delete ui;
}

void ThinningMainWindow::createOperationsToolbar()
{
    QToolBar* opsToolBar = new QToolBar("Operations Tool Bar", this);
    addToolBar(Qt::ToolBarArea::TopToolBarArea, opsToolBar);

    QAction *clearAction = new QAction(tr("Clear Grid"), this);
    clearAction->setShortcut(QKeySequence("Ctrl+D"));
    clearAction->setToolTip("Clear the whole grid");
    connect(clearAction, SIGNAL(triggered()), gridWidget, SLOT(clear()));
    opsToolBar->addAction(clearAction);

    QAction *toggleUpdateAction = new QAction(tr("Toggle Update"), this);
    toggleUpdateAction->setShortcut(QKeySequence("U"));
    toggleUpdateAction->setToolTip("Toggle grid update");
    toggleUpdateAction->setCheckable(true);
    toggleUpdateAction->setChecked(gridWidget->isUpdateEnabled());
    connect(toggleUpdateAction, SIGNAL(triggered()), gridWidget, SLOT(toggleUpdate()));
    opsToolBar->addAction(toggleUpdateAction);

    QAction *oneIterationAction = new QAction(tr("Next Thinning Iteration"), this);
    oneIterationAction->setShortcut(QKeySequence("N"));
    oneIterationAction->setToolTip("Next step or iteration of thinning algorithm");
    connect(oneIterationAction, SIGNAL(triggered()), gridWidget, SLOT(iterate()));
    opsToolBar->addAction(oneIterationAction);

    QAction *oneSubIterationAction = new QAction(tr("Next Thinning Sub-Iteration"), this);
    oneSubIterationAction->setShortcut(QKeySequence("Alt+N"));
    oneSubIterationAction->setToolTip("Next sub-step or sub-iteration of thinning algorithm");
    connect(oneSubIterationAction, SIGNAL(triggered()), gridWidget, SLOT(subIterate()));
    opsToolBar->addAction(oneSubIterationAction);

    QAction *finalResultAction = new QAction(tr("Thinning final result"));
    finalResultAction->setShortcut(QKeySequence("T"));
    finalResultAction->setToolTip("The final result of the thinning algorithm");
    connect(finalResultAction, SIGNAL(triggered()), gridWidget, SLOT(result()));
    opsToolBar->addAction(finalResultAction);
}

void ThinningMainWindow::createAlgorithmsToolbar()
{
    QToolBar *algosToolBar = new QToolBar("Algorithms Tool Bar", this);
    algosToolBar->setOrientation(Qt::Vertical);
    addToolBar(Qt::ToolBarArea::LeftToolBarArea, algosToolBar);

    QActionGroup *algoActionGroup = new QActionGroup(algosToolBar);

    QAction *zhanSuenNWSEAction = algoActionGroup->addAction(tr("ZHANG SUEN NWSE"));
    algosToolBar->addAction(zhanSuenNWSEAction);
    zhanSuenNWSEAction->setCheckable(true);
    zhanSuenNWSEAction->setChecked(gridWidget->getAlgorithm() == ParallelIterativeThinning::ZHANG_SUEN_NWSE);
    connect(zhanSuenNWSEAction, SIGNAL(triggered()), &algorithmSelectionMapper, SLOT(map()));

    QAction *zhanSuenNESWAction = algoActionGroup->addAction(tr("ZHANG SUEN NESW"));
    algosToolBar->addAction(zhanSuenNESWAction);
    zhanSuenNESWAction->setCheckable(true);
    zhanSuenNESWAction->setChecked(gridWidget->getAlgorithm() == ParallelIterativeThinning::ZHANG_SUEN_NESW);
    connect(zhanSuenNESWAction, SIGNAL(triggered()), &algorithmSelectionMapper, SLOT(map()));

    QAction *guoHallAction = algoActionGroup->addAction(tr("GUO HALL"));
    algosToolBar->addAction(guoHallAction);
    guoHallAction->setCheckable(true);
    guoHallAction->setChecked(gridWidget->getAlgorithm() == ParallelIterativeThinning::GUO_HALL);
    connect(guoHallAction, SIGNAL(triggered()), &algorithmSelectionMapper, SLOT(map()));

    QAction *luWangAction = algoActionGroup->addAction(tr("LU WANG"));
    algosToolBar->addAction(luWangAction);
    luWangAction->setCheckable(true);
    luWangAction->setChecked(gridWidget->getAlgorithm() == ParallelIterativeThinning::LU_WANG);
    connect(luWangAction, SIGNAL(triggered()), &algorithmSelectionMapper, SLOT(map()));

    QAction *zhangWangAction = algoActionGroup->addAction(tr("ZHANG WANG"));
    algosToolBar->addAction(zhangWangAction);
    zhangWangAction->setCheckable(true);
    zhangWangAction->setChecked(gridWidget->getAlgorithm() == ParallelIterativeThinning::ZHANG_WANG);
    connect(zhangWangAction, SIGNAL(triggered()), &algorithmSelectionMapper, SLOT(map()));

    QAction *kwkAction = algoActionGroup->addAction(tr("KWK"));
    algosToolBar->addAction(kwkAction);
    kwkAction->setCheckable(true);
    kwkAction->setChecked(gridWidget->getAlgorithm() == ParallelIterativeThinning::KWK);
    connect(kwkAction, SIGNAL(triggered()), &algorithmSelectionMapper, SLOT(map()));

    QAction *hilditchAction = algoActionGroup->addAction(tr("HILDITCH"));
    algosToolBar->addAction(hilditchAction);
    hilditchAction->setCheckable(true);
    hilditchAction->setChecked(gridWidget->getAlgorithm() == ParallelIterativeThinning::HILDITCH);
    connect(hilditchAction, SIGNAL(triggered()), &algorithmSelectionMapper, SLOT(map()));

    QAction *aparajeyaSanyalAction = algoActionGroup->addAction(tr("APARAJEYA SANYAL"));
    algosToolBar->addAction(aparajeyaSanyalAction);
    aparajeyaSanyalAction->setCheckable(true);
    aparajeyaSanyalAction->setChecked(gridWidget->getAlgorithm() == ParallelIterativeThinning::APARAJEYA_SANYAL);
    connect(aparajeyaSanyalAction, SIGNAL(triggered()), &algorithmSelectionMapper, SLOT(map()));

    QAction *boudadSiderTariAction = algoActionGroup->addAction(tr("BOUDAOUD SIDER TARI"));
    algosToolBar->addAction(boudadSiderTariAction);
    boudadSiderTariAction->setCheckable(true);
    boudadSiderTariAction->setChecked(gridWidget->getAlgorithm() == ParallelIterativeThinning::BOUDAOUD_SIDER_TARI);
    connect(boudadSiderTariAction, SIGNAL(triggered()), &algorithmSelectionMapper, SLOT(map()));

    algorithmSelectionMapper.setMapping(zhanSuenNWSEAction, ParallelIterativeThinning::ZHANG_SUEN_NWSE);
    algorithmSelectionMapper.setMapping(zhanSuenNESWAction, ParallelIterativeThinning::ZHANG_SUEN_NESW);
    algorithmSelectionMapper.setMapping(guoHallAction, ParallelIterativeThinning::GUO_HALL);
    algorithmSelectionMapper.setMapping(luWangAction, ParallelIterativeThinning::LU_WANG);
    algorithmSelectionMapper.setMapping(zhangWangAction, ParallelIterativeThinning::ZHANG_WANG);
    algorithmSelectionMapper.setMapping(kwkAction, ParallelIterativeThinning::KWK);
    algorithmSelectionMapper.setMapping(hilditchAction, ParallelIterativeThinning::HILDITCH);
    algorithmSelectionMapper.setMapping(aparajeyaSanyalAction, ParallelIterativeThinning::APARAJEYA_SANYAL);
    algorithmSelectionMapper.setMapping(boudadSiderTariAction, ParallelIterativeThinning::BOUDAOUD_SIDER_TARI);

    connect(&algorithmSelectionMapper, SIGNAL(mapped(int)), gridWidget, SLOT(switchAlgorithm(int)));
    connect(&algorithmSelectionMapper, SIGNAL(mapped(int)), this, SLOT(selectAlgorithm(int)));
}

void ThinningMainWindow::createStatusBar()
{
    statusBar()->showMessage(getAlgoName(gridWidget->getAlgorithm()));
}

QString ThinningMainWindow::getAlgoName(const int idx)
{
    ParallelIterativeThinning::ThinningAlgorithm cur = static_cast<ParallelIterativeThinning::ThinningAlgorithm>(idx);
    switch (cur) {
    case ParallelIterativeThinning::ZHANG_SUEN_NWSE:
    {
        return "ZHANG SUEN NWSE";
    }
    break;
    case ParallelIterativeThinning::ZHANG_SUEN_NESW:
    {
        return "ZHANG SUEN NESW";
    }
    break;
    case ParallelIterativeThinning::GUO_HALL:
    {
        return "GUO HALL";
    }
    break;
    case ParallelIterativeThinning::LU_WANG:
    {
        return "LU WANG";
    }
    break;
    case ParallelIterativeThinning::ZHANG_WANG:
    {
        return "ZHANG WANG";
    }
    break;
    case ParallelIterativeThinning::KWK:
    {
        return "KWK";
    }
    break;
    case ParallelIterativeThinning::HILDITCH:
    {
        return "HILDITCH";
    }
    break;
    case ParallelIterativeThinning::APARAJEYA_SANYAL:
    {
        return "APARAJEYA SANYAL";
    }
    break;
    case ParallelIterativeThinning::BOUDAOUD_SIDER_TARI:
    {
        return "BOUDAOUD SIDER TARI";
    }
    break;
    default:
        return "";
        break;
    }

    return "";
}

void ThinningMainWindow::selectAlgorithm(int idx)
{
    const QString& message = getAlgoName(idx);
    statusBar()->showMessage(message);
}

