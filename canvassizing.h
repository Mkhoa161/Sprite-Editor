#ifndef CANVASSIZING_H
#define CANVASSIZING_H

#include "canvas.h"
#include <QDialog>

namespace Ui {
class CanvasSizing;
}

class CanvasSizing : public QDialog
{
    Q_OBJECT

public:
    explicit CanvasSizing(QWidget *parent = nullptr);
    ~CanvasSizing();

signals:
    void applyClicked(int newSideLength);

private:
    Ui::CanvasSizing *ui;

private slots:
    void onApplyClicked();
    void onSpinBoxValueChanged(int value);
};

#endif // CANVASSIZING_H
