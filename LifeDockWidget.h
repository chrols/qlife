#pragma once

#include <QDockWidget>

#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QGridLayout>

class LifeDockWidget : public QWidget {
    Q_OBJECT
public:
    explicit LifeDockWidget(QWidget *parent = nullptr);
    virtual ~LifeDockWidget();
signals:
    void newWidth(int width);
    void newHeight(int height);
    void newDepth(int depth);

    void newMinAlive(int minAlive);
    void newMaxAlive(int maxAlive);
    void newMinBirth(int minBirth);
    void newMaxBirth(int maxBirth);
private:
    QLabel *m_widthLabel;
    QLabel *m_heightLabel;
    QLabel *m_depthLabel;

    QLabel *m_minBirthLabel;
    QLabel *m_maxBirthLabel;
    QLabel *m_minAliveLabel;
    QLabel *m_maxAliveLabel;

    QSpinBox *m_widthSpinner;
    QSpinBox *m_heightSpinner;
    QSpinBox *m_depthSpinner;

    QSpinBox *m_minBirthSpinner;
    QSpinBox *m_maxBirthSpinner;
    QSpinBox *m_minAliveSpinner;
    QSpinBox *m_maxAliveSpinner;

    QVBoxLayout *m_layout;
    QGridLayout *m_ruleLayout;

    QSpacerItem *m_spacer;
};
