#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->canvas->resize(400, 400);

    toolButtonGroup = new QButtonGroup(this);
    toolButtonGroup->addButton(ui->penButton, 0); // numbers depending on the enum
    toolButtonGroup->addButton(ui->eraserButton, 1);
    toolButtonGroup->addButton(ui->sphereShapeButton, 2);
    toolButtonGroup->addButton(ui->filledSphereShapeButton, 3);
    toolButtonGroup->addButton(ui->boxShapeButton, 4);
    toolButtonGroup->addButton(ui->filledBoxShapeButton, 5);
    toolButtonGroup->addButton(ui->triangleShapeButton, 6);
    toolButtonGroup->addButton(ui->filledTriangleShapeButton, 7);

    // TODO: if pen and eraser in the toolbar need not to be checkable, then delete
    // count also the pen and eraser in the toolbar
    //QToolButton* actionPenButton = qobject_cast<QToolButton*>(
    //    ui->toolBar->widgetForAction(ui->actionPen));
    //QToolButton* actionEraserButton = qobject_cast<QToolButton*>(
    //    ui->toolBar->widgetForAction(ui->actionEraser));
    //toolButtonGroup->addButton(actionPenButton, 0);
    //toolButtonGroup->addButton(actionEraserButton, 1);

    //toolButtonGroup->setExclusive(true);

    connect(ui->actionPen,
            &QAction::triggered,
            this,
            [this]() {
                ui->penButton->setChecked(true);
            });

    connect(ui->actionEraser,
            &QAction::triggered,
            this,
            [this]() {
                ui->eraserButton->setChecked(true);
            });

    connect(this, &MainWindow::toolSelected, ui->canvas, &Canvas::selectTool);

    connect(toolButtonGroup,
            QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
            this,
            [this](QAbstractButton* button){
                int id = toolButtonGroup -> id(button);
                emit toolSelected(static_cast<Canvas::Mode>(id));
            });
}

MainWindow::~MainWindow()
{
    delete ui;
    delete toolButtonGroup;
}

void MainWindow::on_penButton_clicked(bool checked)
{

}

