#ifndef BILATERALFILTERWIDGET_H
#define BILATERALFILTERWIDGET_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QImage>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>

class BilateralFilterWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT

public:
  explicit BilateralFilterWidget(QWidget *parent = Q_NULLPTR);
  void setImage(QImage*);
  void setColorParam(float);
  void setLocationParam(float);
  ~BilateralFilterWidget();
  void setImage(QImage const&);

signals:
  void glInited();

protected:
  void initializeGL() Q_DECL_OVERRIDE;
  void resizeGL(int, int) Q_DECL_OVERRIDE;
  void paintGL() Q_DECL_OVERRIDE;

private:
  QOpenGLShaderProgram program;
  QOpenGLBuffer buffer;
  QOpenGLVertexArrayObject vao;
  QOpenGLTexture *texture;
  QOpenGLBuffer indexBuf;
};

#endif // BILATERALFILTERWIDGET_H
