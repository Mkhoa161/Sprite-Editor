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
        "    color: gray;"
        "    background-color: #f0f0f0;"
        "}"
        "QPushButton {"
        "    color: gray;"
        "    background-color: #e0e0e0;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #d0d0d0;"
        "}"
        );
}

CanvasSizing::~CanvasSizing()
{
    delete ui;
}

void CanvasSizing::onApplyClicked(){
    emit applyClicked(ui->spinBox->value());

    close();
}

void CanvasSizing::onSpinBoxValueChanged(int value){
    if(value <= 0){
        ui->buttonBox->setEnabled(false);
        ui->buttonBox->setStyleSheet(
            "QDialogButtonBox {"
            "    color: gray;"
            "    background-color: #f0f0f0;"
            "}"
            "QPushButton {"
            "    color: gray;"
            "    background-color: #e0e0e0;"
            "}"
            "QPushButton:pressed {"
            "    background-color: #d0d0d0;"
            "}"
            );
    }
    else{
        ui->buttonBox->setEnabled(true);
        ui->buttonBox->setStyleSheet("");
    }
}
