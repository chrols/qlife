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
signals:
    void newStepRequest();

protected:
    // QOpengGLWidget interface
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    QThread m_workerThread;

    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;
    GLuint m_mvpUniform;

    QOpenGLShaderProgram *m_program;
    int m_frame;

    Life m_life;
};
