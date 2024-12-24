// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef BACKEND_H
#define BACKEND_H

#include "animatedparam.h"

#include <QFileDialog>
#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <qqmlregistration.h>

//! [class definition]
class Backend : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int rotation1Angle READ rotation1Angle WRITE setRot1Angle NOTIFY rot1AngleChanged)
    Q_PROPERTY(int rotation2Angle READ rotation2Angle WRITE setRot2Angle NOTIFY rot2AngleChanged)
    Q_PROPERTY(int rotation3Angle READ rotation3Angle WRITE setRot3Angle NOTIFY rot3AngleChanged)
    Q_PROPERTY(int rotation4Angle READ rotation4Angle WRITE setRot4Angle NOTIFY rot4AngleChanged)
    Q_PROPERTY(int clawsAngle READ clawsAngle WRITE setClawsAngle NOTIFY clawsAngleChanged)
    Q_PROPERTY(QString status READ status BINDABLE bindableStatus)
    Q_PROPERTY(QString scriptPath READ scriptPath WRITE setScriptPath NOTIFY scriptPathChanged)
    //! [class definition]

public:
    explicit Backend(QObject *parent = nullptr);

    int rotation1Angle() const;
    void setRot1Angle(const int angle);

    int rotation2Angle() const;
    void setRot2Angle(const int angle);

    int rotation3Angle() const;
    void setRot3Angle(const int angle);

    int rotation4Angle() const;
    void setRot4Angle(const int angle);

    int clawsAngle() const;
    void setClawsAngle(const int angle);

    QString scriptPath() const;
    void setScriptPath(const QString &path);

    QString status() const;
    QBindable<QString> bindableStatus() const;
    Q_INVOKABLE void connectToRobot(const QString &ipAddress);

    Q_INVOKABLE void runProcess();

    Q_INVOKABLE void selectFile()
    {
        QString filePath = QFileDialog::getOpenFileName();
        if (!filePath.isEmpty()) {
            setScriptPath(filePath);
        }
    }

signals:
    void rot1AngleChanged();
    void rot2AngleChanged();
    void rot3AngleChanged();
    void rot4AngleChanged();
    void clawsAngleChanged();
    void scriptPathChanged();
    void errorOccurred(const QString &error);
    void processOutput(const QString &output);

private:
    AnimatedParam m_rotation1Angle;
    AnimatedParam m_rotation2Angle;
    AnimatedParam m_rotation3Angle;
    AnimatedParam m_rotation4Angle;
    AnimatedParam m_clawsAngle;

    QProperty<QString> m_status;
    QProperty<bool> m_isCollision;

    QTcpSocket *socket;
    QTimer *rotationTimer;
    QString m_scriptPath;

    void sendRotationValues();
    void onDisconnected();

    void detectCollision();
};

#endif // BACKEND_H
