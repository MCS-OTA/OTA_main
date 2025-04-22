#ifndef CAMERA_BACKEND_H
#define CAMERA_BACKEND_H

#include <QObject>
#include <QImage>
#include <QTimer>
#include <QQuickImageProvider>
#include <opencv2/opencv.hpp>

// QML에서 사용할 이미지 프로바이더 클래스
class CameraImageProvider : public QQuickImageProvider {
public:
    CameraImageProvider() : QQuickImageProvider(QQuickImageProvider::Image) {}

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override {
        Q_UNUSED(id);
        Q_UNUSED(requestedSize);

        if (size) *size = m_image.size();
        return m_image;
    }

    void updateImage(const QImage &image) {
        m_image = image;
    }

private:
    QImage m_image;
};

// 카메라 백엔드 클래스
class CameraBackend : public QObject {
    Q_OBJECT

public:
    explicit CameraBackend(QObject *parent = nullptr);
    ~CameraBackend();

public slots:
    void updateFrame();

signals:
    void frameChanged();

public:
    CameraImageProvider imageProvider; // 이미지 프로바이더를 포함

private:
    QTimer m_timer;
    cv::VideoCapture cap;
};

#endif // CAMERA_BACKEND_H
