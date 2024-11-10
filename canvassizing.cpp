#include "canvassizing.h"
#include "ui_canvassizing.h"
#include <QDialogButtonBox>
#include <QPushButton>
#include <QSpinBox>

#include <QDebug>

CanvasSizing::CanvasSizing(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CanvasSizing)
{
    ui->setupUi(this);

    connect(ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &CanvasSizing::onApplyClicked);
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &CanvasSizing::reject);
    connect(ui->spinBox, &QSpinBox::valueChanged, this, &CanvasSizing::onSpinBoxValueChanged);

    ui->buttonBox->setStyleSheet(
        "QDialogButtonBox {"
        "    color: gray;"               // Disabled text color
        "    background-color: #f0f0f0;" // Light gray background
        "}"
        "QPushButton {"
        "    color: gray;"               // Disabled text color
        "    background-color: #e0e0e0;" // Light gray button background
        "}"
        "QPushButton:pressed {"
        "    background-color: #d0d0d0;" // Darker gray when pressed
        "}"
        );
}

CanvasSizing::~CanvasSizing()
{
    delete ui;
}

void CanvasSizing::onApplyClicked(){
    if(ui->spinBox->value() <= 0){

    }

    emit applyClicked(ui->spinBox->value());

    close();
}

void CanvasSizing::onSpinBoxValueChanged(int value){
    if(value <= 0){
        ui->buttonBox->setEnabled(false);
        ui->buttonBox->setStyleSheet(
            "QDialogButtonBox {"
            "    color: gray;"               // Disabled text color
            "    background-color: #f0f0f0;" // Light gray background
            "}"
            "QPushButton {"
            "    color: gray;"               // Disabled text color
            "    background-color: #e0e0e0;" // Light gray button background
            "}"
            "QPushButton:pressed {"
            "    background-color: #d0d0d0;" // Darker gray when pressed
            "}"
            );
    }
    else{
        ui->buttonBox->setEnabled(true);
        ui->buttonBox->setStyleSheet("");
    }
}
