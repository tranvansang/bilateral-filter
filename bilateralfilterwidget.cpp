#include "bilateralfilterwidget.h"
#include <QVector>

typedef struct {
  QVector2D position;
  QVector2D textcoord;
} VertexData;

BilateralFilterWidget::BilateralFilterWidget(QWidget *parent)
  : QOpenGLWidget(parent),
    texture(Q_NULLPTR),
    indexBuf(QOpenGLBuffer::IndexBuffer)
{
}

BilateralFilterWidget::~BilateralFilterWidget(){
  makeCurrent();
  vao.destroy();
  buffer.destroy();
  indexBuf.destroy();
  if (texture != Q_NULLPTR) delete texture;
  doneCurrent();
}

void BilateralFilterWidget::initializeGL(){
  initializeOpenGLFunctions();
  //background
  glClearColor(0, 0, 0, 1);

  //init program
  if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.vert")){
      qCritical() << program.log();
      close();
    }
  if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.frag")){
      qCritical() << program.log();
      close();
    }
  if (!program.link()){
      qCritical() << program.log();
      close();
    }
  if (!program.bind()){
      qCritical() << program.log();
      close();
    }

  // buffer
  if (!buffer.create()) close();
  if (!buffer.bind()) close();
  buffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
  QVector<VertexData> rect({
                             {QVector2D(-1.f, -1.f), QVector2D(0.0f, 0.f)},
                             {QVector2D(-1.f, 1.f), QVector2D(0.f, 1.f)},
                             {QVector2D(1.f, 1.f), QVector2D(1.f, 1.f)},
                             {QVector2D(1.f, -1.f), QVector2D(1.f, 0.f)}
                           });
  buffer.allocate(rect.data(), rect.length() * sizeof(VertexData));
  //buffer.allocate()

  //vao
  if (!vao.create()) close();
  vao.bind();

  //bind attributes
  int vertexLocation = program.attributeLocation("a_position");
  program.enableAttributeArray(vertexLocation);
  program.setAttributeBuffer(vertexLocation, GL_FLOAT, offsetof(VertexData, position), 2, sizeof(VertexData));
  // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
  int texcoordLocation = program.attributeLocation("a_texcoord");
  program.enableAttributeArray(texcoordLocation);
  program.setAttributeBuffer(texcoordLocation, GL_FLOAT, offsetof(VertexData, textcoord), 2, sizeof(VertexData));

  vao.release();
  buffer.release();
  program.release();

  //bind index
  indexBuf.create();
  indexBuf.bind();
  QVector<ushort> indices({0, 1, 2, 3, 0, 1, 2, 3});
  indexBuf.allocate(indices.data(), indices.length() * sizeof(ushort));
  indexBuf.release();

  emit glInited();
}

void BilateralFilterWidget::resizeGL(int w, int h){
  program.bind();
  glViewport(0, 0, w, h);

  //setup mvp
  QMatrix4x4 projection;
  projection.setToIdentity();
  projection.scale((float)h/qMax(w, h), (float)w/qMax(w, h), 1);
  program.setUniformValue("mvp_matrix", projection);

  program.release();
}

void BilateralFilterWidget::setColorParam(float sigmaL){
  if (!isValid()) return;
  if (!program.bind()) close();
  program.setUniformValue("sigmaL", sigmaL);
  program.release();
  update();
}

void BilateralFilterWidget::setLocationParam(float sigmaS){
  if (!isValid()) return;
  if (!program.bind()) close();
  program.setUniformValue("sigmaS", sigmaS);
  program.release();
  update();
}

void BilateralFilterWidget::paintGL(){
  if (!program.bind()) close();
  if (texture != Q_NULLPTR) texture->bind();
  vao.bind();
  program.setUniformValue("texture", 0);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  indexBuf.bind();
  glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_SHORT, 0);
  indexBuf.release();

  vao.release();
  program.release();
}

void BilateralFilterWidget::setImage(QImage const& img){
  if (texture != Q_NULLPTR){
      delete texture;
      texture = Q_NULLPTR;
    }
  makeCurrent();
  texture = new QOpenGLTexture(img.mirrored());
  // Set nearest filtering mode for texture minification
  texture->setMinificationFilter(QOpenGLTexture::Nearest);

  // Set bilinear filtering mode for texture magnification
  texture->setMagnificationFilter(QOpenGLTexture::Linear);

  texture->setWrapMode(QOpenGLTexture::Repeat);
  doneCurrent();
}
