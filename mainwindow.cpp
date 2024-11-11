#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "framemanager.h"
#include <QTimer>

MainWindow::MainWindow(FrameManager& frameManager, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    frameLabels = ui->scrollAreaWidgetContents->findChildren<QLabel*>();
    frameLabels[0]->installEventFilter(this);

    // Add actions to a group so that they're exclusive
    toolButtonGroup = new QButtonGroup(this);
    toolButtonGroup->addButton(ui->penButton, 0); // numbers depending on the enum
    toolButtonGroup->addButton(ui->eraserButton, 1);
    toolButtonGroup->addButton(ui->sphereShapeButton, 2);
    toolButtonGroup->addButton(ui->filledSphereShapeButton, 3);
    toolButtonGroup->addButton(ui->boxShapeButton, 4);
    toolButtonGroup->addButton(ui->filledBoxShapeButton, 5);
    toolButtonGroup->addButton(ui->triangleShapeButton, 6);
    toolButtonGroup->addButton(ui->filledTriangleShapeButton, 7);

    // Pen and eraser in the toolbar(not checkable)
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

    // Turning on or off mirror mode
    connect(ui->actionMirror,
            &QAction::toggled,
            ui->canvas,
            &Canvas::setMirrorMode);


    connect(this, &MainWindow::toolSelected, ui->canvas, &Canvas::selectTool);

    // Changes modes of the selected tool in canvas
    connect(toolButtonGroup,
            QOverload<QAbstractButton*, bool>::of(&QButtonGroup::buttonToggled),
            this,
            [this](QAbstractButton* button, bool checked){
                if (checked) {
                    int id = toolButtonGroup->id(button);
                    emit toolSelected(static_cast<Canvas::Mode>(id));
                }
            });

    // Handle color changes
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

    // The timer defers these calls until after the constructor so all the signals can be... signaled
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

    // Pixel drawing
    connect(ui->canvas, &Canvas::paint, &frameManager, &FrameManager::onPainted);
    connect(&frameManager, &FrameManager::selectedFrameChanged, ui->canvas, &Canvas::onSelectedFrameChanged);
    connect(this, &MainWindow::frameAdded, &frameManager, &FrameManager::onFrameAdded);
    connect(&frameManager, &FrameManager::sideLengthChanged, ui->canvas, &Canvas::onSideLengthChanged);

    // Frame previews
    connect(&frameManager, &FrameManager::framesChanged, this, &MainWindow::updateFramePreviews);
    connect(&frameManager, &FrameManager::frameCountChanged, this, &MainWindow::frameCountChanged);
    connect(ui->frameSpinBox, &QSpinBox::valueChanged, &frameManager, &FrameManager::onFrameSelect);
    connect(this, &MainWindow::frameSelect, &frameManager, &FrameManager::onFrameSelect);
    connect(&frameManager, &FrameManager::selectFrameSignal, this, &MainWindow::onSelectFrame);

    frameManager.onFrameAdded();
    frameManager.setSideLength(16);
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


void MainWindow::on_addFrameButton_clicked()
{
    emit frameAdded();
}

void MainWindow::frameCountChanged(int newFrameCount){
    ui->frameSlider->setMaximum(newFrameCount - 1);
    ui->frameSpinBox->setMaximum(newFrameCount - 1);
}

void MainWindow::onSelectFrame(int index){
    frameLabels[index]->setStyleSheet("QLabel { border: 2px solid #2196F3; }");
    if (selectedFrameIndex >= 0 && selectedFrameIndex <= frameLabels.size() - 1){
        frameLabels[selectedFrameIndex]->setStyleSheet("QLabel { border: 2px solid transparent; }");
    }
    selectedFrameIndex = index;
}

void MainWindow::updateFramePreviews(const std::vector<Frame*>& frames) {

    QWidget* scrollContent = ui->scrollAreaWidgetContents;
    QHBoxLayout* layout = qobject_cast<QHBoxLayout*>(scrollContent->layout());

    for (size_t i = 0; i < frames.size(); ++i) {
        QLabel* label;
        if (i < frameLabels.size()) { // check & replace existing lables
            label = frameLabels[i];
        } else { // no exsiting lables, create new ones
            label = new QLabel(scrollContent);
            label->setFixedSize(80, 80);
            label->installEventFilter(this);  // install click selector
            layout->insertWidget(layout->count() - 1, label);
        }
        QPixmap scaledPixmap = frames[i]->pixmap.scaled(80, 80, Qt::KeepAspectRatio);
        label->setPixmap(scaledPixmap);
    }

    // delete excessive frames
    while (frameLabels.size() > frames.size()) {
        QLabel* label = frameLabels.takeLast();
        layout->removeWidget(label);
        delete label;
    }

    //renew frameLabels for indexing
    frameLabels = scrollContent->findChildren<QLabel*>();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QLabel* label = qobject_cast<QLabel*>(obj);
        if (label && frameLabels.contains(label)) {
            int clickedIndex = frameLabels.indexOf(label);
            emit frameSelect(clickedIndex);
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}
