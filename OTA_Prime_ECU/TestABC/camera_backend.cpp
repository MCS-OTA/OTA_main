#include "camera_backend.h"
#include <QDebug>

CameraBackend::CameraBackend(QObject *parent) : QObject(parent) {
    cap.open(0, cv::CAP_V4L2); // PiCamera 열기
    if (!cap.isOpened()) {
        qWarning() << "PiCamera를 열 수 없습니다!";
        return;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    connect(&m_timer, &QTimer::timeout, this, &CameraBackend::updateFrame);
    m_timer.start(30);
}

CameraBackend::~CameraBackend() {
    m_timer.stop();
    cap.release();
}

void CameraBackend::updateFrame() {
    if (!cap.isOpened()) return;
    cv::Mat frame;
    cap >> frame;

    if (frame.empty()) return;

    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

    QImage img(frame.cols, frame.rows, QImage::Format_RGB888);
    memcpy(img.bits(), frame.data, frame.total() * frame.elemSize());

//    QImage img(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);

    imageProvider.updateImage(img);

    emit frameChanged();
}
