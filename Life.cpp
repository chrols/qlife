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
      m_wrap(false),
      m_useFirst(true) {}

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

void Life::step() {
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
    randomize();
}

void Life::setRules(int minAlive, int maxAlive, int minBirth, int maxBirth) {
    m_minAlive = minAlive;
    m_maxAlive = maxAlive;
    m_minBirth = minBirth;
    m_maxBirth = maxBirth;
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
