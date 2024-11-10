#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "framemanager.h"
#include <QTimer>

MainWindow::MainWindow(FrameManager& frameManager, QWidget *parent)
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

    //Changes modes of the selected tool in canvas
    connect(toolButtonGroup,
            QOverload<QAbstractButton*, bool>::of(&QButtonGroup::buttonToggled),
            this,
            [this](QAbstractButton* button, bool checked){
                if (checked) {
                    int id = toolButtonGroup->id(button);
                    emit toolSelected(static_cast<Canvas::Mode>(id));
                }
            });

    //Handle color changes
    connect(ui->spinBox_red, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::emitColorChange);
    connect(ui->slider_red, &QSlider::valueChanged, this, &MainWindow::emitColorChange);
    connect(ui->spinBox_green, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::emitColorChange);
    connect(ui->slider_green, &QSlider::valueChanged, this, &MainWindow::emitColorChange);
    connect(ui->spinBox_blue, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::emitColorChange);
    connect(ui->slider_blue, &QSlider::valueChanged, this, &MainWindow::emitColorChange);
    connect(ui->spinBox_alpha, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::emitColorChange);
    connect(ui->slider_alpha, &QSlider::valueChanged, this, &MainWindow::emitColorChange);
    connect(ui->colorHex, &QTextEdit::textChanged, this, &MainWindow::emitHexChange);
    connect(this, &MainWindow::setCurrentColor, ui->canvas, &Canvas::setCurrentColor);

    //The timer defers these calls until after the constructor so all the signals can be... signaled
    QTimer::singleShot(0, this, [this]() {
        Canvas::Mode defaultMode = ui->canvas->DEFAULT_MODE;
        QColor defaultColor = ui->canvas->DEFAULT_COLOR;
        QAbstractButton* defaultButton = toolButtonGroup->button(static_cast<int>(defaultMode));
        if (defaultButton) {
            defaultButton->setChecked(true);
        }
        ui->spinBox_red->setValue(defaultColor.red());
        ui->spinBox_green->setValue(defaultColor.green());
        ui->spinBox_blue->setValue(defaultColor.blue());
        ui->spinBox_alpha->setValue(defaultColor.alpha());
    });

    //connect(ui->canvas, &Canvas::paint, &frameManager, &FrameManager::paintCurrentFrame);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete toolButtonGroup;
}

void MainWindow::emitColorChange()
{
    int r = ui->spinBox_red->value();
    int g = ui->spinBox_green->value();
    int b = ui->spinBox_blue->value();
    int a = ui->spinBox_alpha->value();
    QColor rgbToHex(r, g, b, a);

    // Temporarily block signals to avoid recursion
    ui->colorHex->blockSignals(true);
    ui->colorHex->setPlainText(rgbToHex.name(QColor::HexArgb));
    ui->colorHex->blockSignals(false);
    updateColorPreview(rgbToHex);

    emit setCurrentColor(r, g, b, a);
}
void MainWindow::emitHexChange()
{
    QString hex = ui->colorHex->toPlainText();
    QColor hexToRGB(hex);

    if (!hexToRGB.isValid()) {
        return;
    }

    // Temporarily block signals to avoid recursion
    ui->spinBox_red->blockSignals(true);
    ui->spinBox_green->blockSignals(true);
    ui->spinBox_blue->blockSignals(true);
    ui->spinBox_alpha->blockSignals(true);

    ui->spinBox_red->setValue(hexToRGB.red());
    ui->spinBox_green->setValue(hexToRGB.green());
    ui->spinBox_blue->setValue(hexToRGB.blue());
    ui->spinBox_alpha->setValue(hexToRGB.alpha());

    ui->spinBox_red->blockSignals(false);
    ui->spinBox_green->blockSignals(false);
    ui->spinBox_blue->blockSignals(false);
    ui->spinBox_alpha->blockSignals(false);

    emitColorChange();
}

void MainWindow::updateColorPreview(QColor color)
{

    QString colorString = color.name(QColor::HexArgb);
    ui->colorPreview->setStyleSheet(QString("background-color: %1;").arg(colorString));
}

