#include "bilateralfiltering.h"
#include "ui_bilateralfiltering.h"
#include <QFileDialog>

BilateralFiltering::BilateralFiltering(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::BilateralFiltering),
  maxColorParam(5.f),
  maxLocationParam(20.f)
{
  ui->setupUi(this);
  ui->saveButton->setDisabled(true);
  connect(ui->bilateralFilterWidget, &BilateralFilterWidget::glInited, [=](){
      on_colorParamSlider_valueChanged(ui->colorParamSlider->value());
      on_locationParamSlider_valueChanged(ui->locationParamSlider->value());
      loadImage(":/face_display.png");
    });
}

BilateralFiltering::~BilateralFiltering()
{
  delete ui;
}

void BilateralFiltering::on_loadImageButton_clicked()
{
  ui->messageLabel->setText("Loading image...");
  QString filePath = QFileDialog::getOpenFileName(this, tr("Open image to be filtered"), QDir::currentPath(), tr(""), 0, QFileDialog::ReadOnly);
  loadImage(filePath);
}

void BilateralFiltering::loadImage(QString filePath){
  QImage image;
  if (!filePath.isNull()){
      if (image.load(filePath)){
          qInfo() << "Load image ok";
          ui->messageLabel->setText("Load image ok");
          QPixmap pixmap;
          pixmap.convertFromImage(image);
          ui->originalImageLabel->setPixmap(pixmap.scaled(ui->originalImageLabel->width(), ui->originalImageLabel->height(), Qt::KeepAspectRatio));

          QImage scaledImage(image.scaled(ui->bilateralFilterWidget->width(), ui->bilateralFilterWidget->height(), Qt::KeepAspectRatio));
          ui->saveButton->setDisabled(false);
          QSize topleft = (ui->bilateralFilterWidget->size() - scaledImage.size()) / 2;
          imageRect = QRect(topleft.width(), topleft.height(), scaledImage.width(), scaledImage.height());
          ui->bilateralFilterWidget->setImage(image);
          ui->bilateralFilterWidget->update();
        }else{
          qWarning() << "Cannot load image";
          ui->messageLabel->setText("Load image failed");
        }
    }else{
      ui->messageLabel->setText("");
    }
}

float BilateralFiltering::colorParam(){
  return  maxColorParam * ui->colorParamSlider->value() / ui->colorParamSlider->maximum();
}

float BilateralFiltering::locationParam(){
  return  maxLocationParam * ui->locationParamSlider->value() / ui->locationParamSlider->maximum();
}

void BilateralFiltering::on_colorParamSlider_valueChanged(int value)
{
  ui->colorParamLabel->setText(tr("").sprintf("%.1f", colorParam()));
  ui->bilateralFilterWidget->setColorParam(colorParam());
}

void BilateralFiltering::on_locationParamSlider_valueChanged(int value)
{
  ui->locationParamLabel->setText(tr("").sprintf("%.1f", locationParam()));
  ui->bilateralFilterWidget->setLocationParam(locationParam());
}

void BilateralFiltering::on_saveButton_clicked()
{
  QString filePath = QFileDialog::getSaveFileName(this, tr("Save as JPG image"), QDir::currentPath(), tr(""), 0, 0);
  QString ext = ".jpg";
  if (filePath.length() <= ext.length() || filePath.lastIndexOf(ext, -1, Qt::CaseInsensitive) != filePath.length() - ext.length())
    filePath = filePath + ".jpg";
  if (!filePath.isNull()){
      QPixmap image = QPixmap::grabWidget(ui->bilateralFilterWidget, imageRect);
      image.save(filePath, "JPG");
    }
}
