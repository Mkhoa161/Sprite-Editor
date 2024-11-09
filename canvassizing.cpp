#include "canvassizing.h"
#include "ui_canvassizing.h"
#include <QDialogButtonBox>
#include <QPushButton>

CanvasSizing::CanvasSizing(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CanvasSizing)
{
    ui->setupUi(this);

    connect(ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &CanvasSizing::onApplyClicked);
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &CanvasSizing::reject);
}

CanvasSizing::~CanvasSizing()
{
    delete ui;
}

void CanvasSizing::onApplyClicked(){
    //TODO: apply the new canvas size to the frame manager.

    close();
}
