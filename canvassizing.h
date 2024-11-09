#ifndef CANVASSIZING_H
#define CANVASSIZING_H

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

private:
    Ui::CanvasSizing *ui;

private slots:
    void onApplyClicked();
};

#endif // CANVASSIZING_H
