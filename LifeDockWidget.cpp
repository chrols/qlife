#include "LifeDockWidget.h"

#include <QAction>
#include <QGridLayout>
#include <QSpinBox>
#include <QSettings>

LifeDockWidget::LifeDockWidget(QWidget *parent)
 : QWidget(parent)
{
    QSettings settings;

    m_layout = new QVBoxLayout(this);

    m_ruleLayout = new QGridLayout();

    m_widthLabel = new QLabel("Width", this);
    m_heightLabel = new QLabel("Height", this);
    m_depthLabel = new QLabel("Depth", this);

    m_minAliveLabel = new QLabel("Underpopulation", this);
    m_maxAliveLabel = new QLabel("Overpopulation", this);
    m_minBirthLabel = new QLabel("Birth min", this);
    m_maxBirthLabel = new QLabel("Birth max", this);

    m_widthSpinner = new QSpinBox(this);
    m_widthSpinner->setMinimum(1);
    m_heightSpinner = new QSpinBox(this);
    m_heightSpinner->setMinimum(1);
    m_depthSpinner = new QSpinBox(this);
    m_depthSpinner->setMinimum(1);

    m_widthSpinner->setValue(settings.value("sim/width", 30).toInt());
    m_heightSpinner->setValue(settings.value("sim/height", 30).toInt());
    m_depthSpinner->setValue(settings.value("sim/depth", 30).toInt());

    m_minAliveSpinner = new QSpinBox(this);
    m_minAliveSpinner->setMinimum(1);
    m_maxAliveSpinner = new QSpinBox(this);
    m_maxAliveSpinner->setMinimum(1);

    m_minBirthSpinner = new QSpinBox(this);
    m_minBirthSpinner->setMinimum(1);
    m_maxBirthSpinner = new QSpinBox(this);
    m_maxBirthSpinner->setMinimum(1);

    m_minAliveSpinner->setValue(settings.value("sim/min_alive", 5).toInt());
    m_maxAliveSpinner->setValue(settings.value("sim/max_alive", 7).toInt());
    m_minBirthSpinner->setValue(settings.value("sim/min_birth", 6).toInt());
    m_maxBirthSpinner->setValue(settings.value("sim/max_birth", 6).toInt());

    m_ruleLayout->addWidget(m_widthLabel, 0, 0);
    m_ruleLayout->addWidget(m_heightLabel, 1, 0);
    m_ruleLayout->addWidget(m_depthLabel, 2, 0);
    m_ruleLayout->addWidget(m_minAliveLabel, 3, 0);
    m_ruleLayout->addWidget(m_maxAliveLabel, 4, 0);
    m_ruleLayout->addWidget(m_minBirthLabel, 5, 0);
    m_ruleLayout->addWidget(m_maxBirthLabel, 6, 0);

    m_ruleLayout->addWidget(m_widthSpinner, 0, 1);
    m_ruleLayout->addWidget(m_heightSpinner, 1, 1);
    m_ruleLayout->addWidget(m_depthSpinner, 2, 1);
    m_ruleLayout->addWidget(m_minAliveSpinner, 3, 1);
    m_ruleLayout->addWidget(m_maxAliveSpinner, 4, 1);
    m_ruleLayout->addWidget(m_minBirthSpinner, 5, 1);
    m_ruleLayout->addWidget(m_maxBirthSpinner, 6, 1);

    connect(m_widthSpinner, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &LifeDockWidget::newWidth);
    connect(m_heightSpinner, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &LifeDockWidget::newHeight);
    connect(m_depthSpinner, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &LifeDockWidget::newDepth);

    connect(m_minAliveSpinner, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &LifeDockWidget::newMinAlive);
    connect(m_maxAliveSpinner, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &LifeDockWidget::newMaxAlive);
    connect(m_minBirthSpinner, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &LifeDockWidget::newMinBirth);
    connect(m_maxBirthSpinner, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &LifeDockWidget::newMaxBirth);


    m_layout->addLayout(m_ruleLayout);
    m_layout->addStretch();

}

LifeDockWidget::~LifeDockWidget()
{
    m_ruleLayout->deleteLater();
}
