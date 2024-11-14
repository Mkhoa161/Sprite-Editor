/*
    Authors: Zhuyi Bu, Zhenzhi Liu, Justin Melore, Maxwell Rodgers, Duke Nguyen, Minh Khoa Ngo
    Github usernames: 1144761429, 0doxes0, JustinMelore, maxdotr, duke7012, Mkhoa161
    Date: November 10th, 2024
    Class: CS3505, Fall 2024
    Assignment - A8: Sprite Editor Implementation

    The CanvasSizing class serves as the the view of the pop-up window that let users to change the side length/dimension
    of the canvas they are painting on. This file serves as the class's implementation.

    Code style checked by: Justin Melore
*/

#include <QDialogButtonBox>
#include <QPushButton>
#include <QSpinBox>
#include "canvassizing.h"
#include "ui_canvassizing.h"

CanvasSizing::CanvasSizing(QWidget *parent) : QDialog(parent), ui(new Ui::CanvasSizing) {
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

CanvasSizing::~CanvasSizing() {
    delete ui;
}

void CanvasSizing::onApplyClicked() {
    emit applyClicked(ui->spinBox->value());
    close();
}

void CanvasSizing::onSpinBoxValueChanged(int value) {
    if (value <= 0) {
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
    } else {
        ui->buttonBox->setEnabled(true);
        ui->buttonBox->setStyleSheet("");
    }
}
