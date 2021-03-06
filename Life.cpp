#include "Life.h"

#include <cstdlib>
#include <vector>

Life::Life()
    : m_width(0),
      m_height(0),
      m_depth(0),
      m_minAlive(2),
      m_maxAlive(3),
      m_minBirth(3),
      m_maxBirth(3),
      m_running(true),
      m_wrap(false),
      m_useFirst(true) {
}

Life::Life(int width, int height, int depth)
    : m_width(width), m_height(height), m_depth(depth), m_useFirst(true) {
    m_first.assign(width * height * depth, false);
    m_second.assign(width * height * depth, false);
    randomize();
}

int Life::width() const {
    return m_width;
}

int Life::height() const {
    return m_width;
}

int Life::depth() const {
    return m_depth;
}

bool Life::alive(int x, int y, int z) const {
    if (m_wrap) {
        if (x < 0) return alive(m_width + x, y, z);
        if (y < 0) return alive(x, m_height + y, z);
        if (z < 0) return alive(x, y, m_depth + z);
        if (x >= m_width) return alive(x % m_width, y, z);
        if (y >= m_height) return alive(x, y % m_height, z);
        if (z >= m_depth) return alive(x, y, z % m_depth);
    } else {
        if (x < 0 || y < 0 || z < 0 || x >= m_width || y >= m_height ||
            z >= m_depth) {
            return false;
        }
    }

    if (m_useFirst) {
        return m_first[_pos(x, y, z)];
    } else {
        return m_second[_pos(x, y, z)];
    }
}

int Life::neighbours(int x, int y, int z) const {
    int neigh = 0;

    for (int xd = -1; xd <= 1; xd++) {
        for (int yd = -1; yd <= 1; yd++) {
            for (int zd = -1; zd <= 1; zd++) {
                if (xd == 0 && yd == 0 && zd == 0) continue;

                if (alive(x + xd, y + yd, z + zd)) neigh++;
            }
        }
    }

    return neigh;
}

void Life::randomize() {
    for (int i = 0; i < m_width * m_height * m_depth; i++) {
        bool value = (rand() % 5 == 1);

        if (m_useFirst) {
            m_first[i] = value;
        } else {
            m_second[i] = value;
        }
    }
}

void Life::restart() {
    randomize();
    m_running = true;
}

void Life::startStop() {
    m_running = !m_running;
}

void Life::step() {
    if (!m_running) {
        return;
    }

#pragma omp parallell for collapse(3)
    for (int z = 0; z < m_depth; z++) {
        for (int y = 0; y < m_height; y++) {
            for (int x = 0; x < m_width; x++) {
                int neigh = neighbours(x, y, z);

                bool value = false;

                if (alive(x, y, z) &&
                    (neigh >= m_minAlive && neigh <= m_maxAlive)) {
                    value = true;
                } else if (neigh >= m_minBirth && neigh <= m_maxBirth) {
                    value = true;
                }

                if (m_useFirst) {
                    m_second[_pos(x, y, z)] = value;
                } else {
                    m_first[_pos(x, y, z)] = value;
                }
            }
        }
    }

    emit stepReady();

    m_useFirst = !m_useFirst;

    // Might as well restart if we have reached a static state
    for (int i = 0; i < m_first.size(); i++) {
        if (m_first[i] != m_second[i]) {
            return;
        }
    }

    randomize();
}

void Life::resize(int width, int height, int depth) {
    m_first.assign(width * height * depth, false);
    m_second.assign(width * height * depth, false);
    m_width = width;
    m_height = height;
    m_depth = depth;

    m_settings.setValue("sim/width", m_width);
    m_settings.setValue("sim/height", m_height);
    m_settings.setValue("sim/depth", m_depth);

    randomize();
}

void Life::setRules(int minAlive, int maxAlive, int minBirth, int maxBirth) {
    m_minAlive = minAlive;
    m_maxAlive = maxAlive;
    m_minBirth = minBirth;
    m_maxBirth = maxBirth;
    m_settings.setValue("sim/min_alive", m_minAlive);
    m_settings.setValue("sim/max_alive", m_maxAlive);
    m_settings.setValue("sim/min_birth", m_minBirth);
    m_settings.setValue("sim/max_birth", m_maxBirth);

    randomize();
}

int Life::_pos(int x, int y, int z) const {
    return (z * (m_width * m_height) + y * m_width + x);
}

bool Life::wrap() const {
    return m_wrap;
}

void Life::setWrap(bool wrap) {
    m_wrap = wrap;
}

void Life::readSettings()
{
    // FIXME Potentially we can get 0 if the setting data is junk

    int width = m_settings.value("sim/width", 30).toInt();
    int height = m_settings.value("sim/height", 30).toInt();
    int depth = m_settings.value("sim/depth", 30).toInt();

    resize(width, height, depth);

    int minAlive = m_settings.value("sim/min_alive", 5).toInt();
    int maxAlive = m_settings.value("sim/max_alive", 7).toInt();
    int minBirth = m_settings.value("sim/min_birth", 6).toInt();
    int maxBirth = m_settings.value("sim/max_birth", 6).toInt();

    setRules(minAlive, maxAlive, minBirth, maxBirth);
}

void Life::onNewWidth(int width)
{
    resize(width, m_height, m_depth);
    restart();
}

void Life::onNewHeight(int height)
{
    resize(m_width, height, m_depth);
    restart();
}

void Life::onNewDepth(int depth)
{
    resize(m_width, m_height, depth);
    restart();
}

void Life::onNewMinAlive(int minAlive)
{
    setRules(minAlive, m_maxAlive, m_minBirth, m_maxBirth);
    restart();
}

void Life::onNewMaxAlive(int maxAlive)
{
    setRules(m_minAlive, maxAlive, m_minBirth, m_maxBirth);
    restart();
}

void Life::onNewMinBirth(int minBirth)
{
    setRules(m_minAlive, m_maxAlive, minBirth, m_maxBirth);
    restart();
}

void Life::onNewMaxBirth(int maxBirth)
{
    setRules(m_minAlive, m_maxAlive, m_minBirth, maxBirth);
    restart();
}
