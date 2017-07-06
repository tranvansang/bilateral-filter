#ifndef BILATERALFILTERING_H
#define BILATERALFILTERING_H

#include <QMainWindow>
#include <QImage>

namespace Ui {
class BilateralFiltering;
}

class BilateralFiltering : public QMainWindow
{
    Q_OBJECT

public:
    explicit BilateralFiltering(QWidget *parent = 0);
    ~BilateralFiltering();

private slots:
    void on_loadImageButton_clicked();

    void on_colorParamSlider_valueChanged(int value);

    void on_locationParamSlider_valueChanged(int value);

    void on_saveButton_clicked();

private:
    Ui::BilateralFiltering *ui;
    float maxColorParam;
    float maxLocationParam;
    QRect imageRect;
    void loadImage(QString);
    void updateParam();
};

#endif // BILATERALFILTERING_H
