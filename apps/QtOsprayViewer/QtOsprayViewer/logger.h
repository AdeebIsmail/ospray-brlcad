#pragma once

#include <QDebug>

#define LOG_INFO(msg) qDebug() << "[INFO]" << msg;
#define LOG_WARN(msg) qWarning() << "[WARN]" << msg;
#define LOG_ERROR(msg) qCritical() << "[ERROR]" << msg;