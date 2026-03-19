#include "renderwidget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QWheelEvent>
#include <algorithm>
#include <cmath>
#include <cstring>

using rkcommon::math::vec3f;

static inline float fitDistanceFromRadius(float radius, float fovyDeg)
{
  float fovyRad = fovyDeg * 3.1415926535f / 180.0f;
  float halfAngle = 0.5f * fovyRad;

  if (halfAngle <= 0.001f)
    return radius * 3.0f;

  return radius / std::tan(halfAngle) * 1.2f; // 1.2 = padding
}

static inline float clampf(float v, float lo, float hi)
{
  return std::max(lo, std::min(hi, v));
}

static inline vec3f normalizeVec(const vec3f &v)
{
  float len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
  if (len <= 1e-8f)
    return vec3f(0.f, 0.f, 1.f);
  return vec3f(v.x / len, v.y / len, v.z / len);
}

static inline vec3f crossVec(const vec3f &a, const vec3f &b)
{
  return vec3f(
      a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

RenderWidget::RenderWidget(QWidget *parent) : QWidget(parent)
{
  setMouseTracking(true);

  backend_.init();
  backend_.resize(width(), height());

  resetView();
  syncCameraToBackend();
  renderOnce();
}

void RenderWidget::resetView()
{
  center_ = backend_.getBoundsCenter();

  float radius = backend_.getBoundsRadius();
  if (radius < 0.001f)
    radius = 1.0f;

  up_ = vec3f(0.f, 1.f, 0.f);
  yaw_ = 0.3f;
  pitch_ = 0.2f;
  fovy_ = 60.0f;

  dist_ = fitDistanceFromRadius(radius, fovy_);

  backend_.resetAccumulation();
  syncCameraToBackend();
  renderOnce();
}

bool RenderWidget::loadModel(const QString &path)
{
  if (!backend_.loadObj(path.toStdString()))
    return false;

  resetView();
  return true;
}

void RenderWidget::syncCameraToBackend()
{
  pitch_ = clampf(pitch_, -1.4f, 1.4f);

  vec3f eye(center_.x + dist_ * std::cos(pitch_) * std::sin(yaw_),
      center_.y + dist_ * std::sin(pitch_),
      center_.z + dist_ * std::cos(pitch_) * std::cos(yaw_));

  backend_.setCamera(eye, center_, up_, fovy_);
}

void RenderWidget::renderOnce()
{
  const uint32_t *px = backend_.render();

  const int w = backend_.width();
  const int h = backend_.height();

  if (w <= 0 || h <= 0)
    return;

  if (image_.width() != w || image_.height() != h)
    image_ = QImage(w, h, QImage::Format_RGBA8888);

  std::memcpy(image_.bits(), px, size_t(w) * size_t(h) * 4);
  update();
}

void RenderWidget::paintEvent(QPaintEvent *)
{
  QPainter p(this);

  QImage img = image_.rgbSwapped().mirrored(false, true);
  p.drawImage(rect(), img);

  p.setPen(Qt::yellow);
  p.drawText(10, 25, "LMB orbit | RMB pan | wheel zoom");
}

void RenderWidget::resizeEvent(QResizeEvent *)
{
  backend_.resize(width(), height());
  backend_.resetAccumulation();
  syncCameraToBackend();
  renderOnce();
}

void RenderWidget::mousePressEvent(QMouseEvent *e)
{
  lastMouse_ = e->pos();
}

void RenderWidget::mouseMoveEvent(QMouseEvent *e)
{
  const QPoint d = e->pos() - lastMouse_;
  lastMouse_ = e->pos();

  if (e->buttons() & Qt::LeftButton) {
    yaw_ += d.x() * orbitSpeed_;
    pitch_ += d.y() * orbitSpeed_;

    backend_.resetAccumulation();
    syncCameraToBackend();
    renderOnce();
  }

  if (e->buttons() & Qt::RightButton) {
    pitch_ = clampf(pitch_, -1.4f, 1.4f);

    vec3f forward = normalizeVec(vec3f(std::cos(pitch_) * std::sin(yaw_),
        std::sin(pitch_),
        std::cos(pitch_) * std::cos(yaw_)));
    vec3f right = normalizeVec(crossVec(forward, up_));
    vec3f upCam = normalizeVec(crossVec(right, forward));

    float sx = float(d.x()) * panSpeed_ * dist_;
    float sy = float(d.y()) * panSpeed_ * dist_;

    center_ = vec3f(center_.x - right.x * sx + upCam.x * sy,
        center_.y - right.y * sx + upCam.y * sy,
        center_.z - right.z * sx + upCam.z * sy);

    backend_.resetAccumulation();
    syncCameraToBackend();
    renderOnce();
  }
}

void RenderWidget::wheelEvent(QWheelEvent *e)
{
  float steps = e->angleDelta().y() / 120.f;
  if (steps == 0.f)
    return;

  dist_ *= std::pow(zoomFactor_, steps);
  dist_ = clampf(dist_, 0.5f, 50.f);

  backend_.resetAccumulation();
  syncCameraToBackend();
  renderOnce();
}