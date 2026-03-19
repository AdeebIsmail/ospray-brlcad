#pragma once

#include <QImage>
#include <QPoint>
#include <QString>
#include <QWidget>

#include <ospray/ospray_cpp/ext/rkcommon.h>
#include "ospraybackend.h"

class RenderWidget : public QWidget
{
  Q_OBJECT
 public:
  explicit RenderWidget(QWidget *parent = nullptr);

  bool loadModel(const QString &path);
  void resetView();

 protected:
  void paintEvent(QPaintEvent *) override;
  void resizeEvent(QResizeEvent *) override;
  void mousePressEvent(QMouseEvent *e) override;
  void mouseMoveEvent(QMouseEvent *e) override;
  void wheelEvent(QWheelEvent *e) override;

 private:
  void syncCameraToBackend();
  void renderOnce();

  OsprayBackend backend_;
  QImage image_;
  QPoint lastMouse_;

  rkcommon::math::vec3f center_{0.f, 0.f, 1.5f};
  rkcommon::math::vec3f up_{0.f, 1.f, 0.f};

  float yaw_ = 0.3f;
  float pitch_ = 0.2f;
  float dist_ = 4.0f;
  float fovy_ = 60.0f;

  float orbitSpeed_ = 0.01f;
  float panSpeed_ = 0.0025f;
  float zoomFactor_ = 0.9f;
};