#include "LifeWidget.h"

#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QScreen>

#include <QtCore/qmath.h>

LifeWidget::LifeWidget() : m_frame(0) {
    m_life.resize(30, 30, 30);
    m_life.setRules(5, 7, 6, 6);
    m_life.setWrap(false);

    m_life.moveToThread(&m_workerThread);

    connect(&m_workerThread, &QThread::finished, &m_life, &Life::deleteLater);

    connect(this, &LifeWidget::newStepRequest, &m_life, &Life::step);

    m_workerThread.start();

    emit newStepRequest();
}

LifeWidget::~LifeWidget() {
    m_workerThread.quit();
    m_workerThread.wait();
}

void LifeWidget::initializeGL() {
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                       ":shaders/VertexShader.vsh");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                       ":shaders/FragmentShader.fsh");
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");
    m_mvpUniform = m_program->uniformLocation("MVP");
}

void LifeWidget::resizeGL(int w, int h) {}

void LifeWidget::paintGL() {
    QOpenGLFunctions *f = context()->functions();

    const qreal retinaScale = devicePixelRatio();
    f->glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    f->glEnable(GL_DEPTH_TEST);
    f->glDepthFunc(GL_LESS);

    m_program->bind();

    float viewRadius = 3.0f;

    float deg = float((m_frame / 2) % 360);
    float rad = deg * float(2.0f * M_PI / 360.f);
    float cellSize = float(3.0f / m_life.width());
    float xOffset = float(cellSize * m_life.width() / 2);
    float yOffset = float(cellSize * m_life.height() / 2);
    float zOffset = float(cellSize * m_life.depth() / 2);

    for (int y = 0; y < m_life.height(); y++) {
        for (int x = 0; x < m_life.width(); x++) {
            for (int z = 0; z < m_life.depth(); z++) {
                if (!m_life.alive(x, y, z)) continue;

                QMatrix4x4 matrix;
                matrix.translate(cellSize * x - xOffset, cellSize * y - yOffset,
                                 cellSize * z - zOffset);
                matrix.scale(cellSize * 0.5f);

                QMatrix4x4 mvp;
                mvp.perspective(90.0f, 4.0f / 3.0f, 0.1f, 100.0f);
                mvp.lookAt(
                    QVector3D(viewRadius * sin(rad), 0, viewRadius * cos(rad)),
                    QVector3D(0, 0, 0), QVector3D(0, 1, 0));

                m_program->setUniformValue(m_matrixUniform, matrix);
                m_program->setUniformValue(m_mvpUniform, mvp);

                GLfloat vertices[] = {
                    -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,
                    1.0f,  1.0f,  1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,
                    1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f,
                    1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,
                    -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,
                    1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,
                    -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
                    1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,
                    1.0f,  1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f,
                    1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  -1.0f,
                    1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, -1.0f,
                    1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,
                    -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,
                    1.0f,  1.0f,  -1.0f, 1.0f};

                GLfloat *colors = new GLfloat[108];

                float r1 = float(x) / float(m_life.width());
                float g1 = float(y) / float(m_life.height());
                float b1 = float(z) / float(m_life.depth());
                float r2 = float(x + 1) / float(m_life.width());
                float g2 = float(y + 1) / float(m_life.height());
                float b2 = float(z + 1) / float(m_life.depth());

                for (int i = 0; i < 108; i += 3) {
                    colors[i] = vertices[i] < 0 ? r1 : r2;
                    colors[i + 1] = vertices[i + 1] < 0 ? g1 : g2;
                    colors[i + 2] = vertices[i + 2] < 0 ? b1 : b2;
                }

                f->glUniformMatrix4fv(m_mvpUniform, 1, GL_FALSE,
                                      mvp.constData());
                f->glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0,
                                         vertices);
                f->glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0,
                                         colors);

                f->glEnableVertexAttribArray(0);
                f->glEnableVertexAttribArray(1);

                f->glDrawArrays(GL_TRIANGLES, 0, 3 * 12);

                f->glDisableVertexAttribArray(1);
                f->glDisableVertexAttribArray(0);
            }
        }
    }
    m_program->release();

    m_frame++;

    if (m_frame % 10 == 0) {
        emit newStepRequest();
    }

    update();
}
