#pragma once

#include <QElapsedTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QThread>
#include "Life.h"

class LifeWidget : public QOpenGLWidget {
    Q_OBJECT
public:
    explicit LifeWidget();
    virtual ~LifeWidget();
public slots:
    void startStopCamera();
signals:
    void restartSimulation();
    void startStopSimulation();
    void stepSimulation();

protected:
    // QOpengGLWidget interface
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    void _updateColor(int x, int y, int z);

    QThread m_workerThread;

    GLuint m_posAttribute;
    GLuint m_colorAttribute;
    GLuint m_matrixUniform;
    GLuint m_mvpUniform;

    QVector<GLfloat> m_cubeVertices;
    QVector<GLfloat> m_cubeColor;

    QOpenGLShaderProgram *m_program;
    int m_frame;

    float m_cameraAngle;

    bool m_cameraRunning;

    Life *m_life;
};
