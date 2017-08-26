#pragma once

#include <QObject>

#include <cstdint>
#include <vector>

class Life : public QObject {
    Q_OBJECT
public:
    Life();
    Life(int width, int height, int depth);

    int width() const;
    int height() const;
    int depth() const;

    bool alive(int x, int y, int z) const;
    int neighbours(int x, int y, int z) const;
    void randomize();

    void resize(int width, int height, int depth);

    // Common rules:
    // 2333 "Conway
    // 4555 3D alternative 1
    // 5766 3D alternative 2
    void setRules(int minAlive, int maxAlive, int minBirth, int maxBirth);
    uint32_t value(int x, int y, int z) const;
    bool wrap() const;
    void setWrap(bool wrap);
public slots:
    void step();
signals:
    void stepReady();

private:
    int _pos(int x, int y, int z) const;

    int m_width;
    int m_height;
    int m_depth;

    int m_minAlive;
    int m_maxAlive;
    int m_minBirth;
    int m_maxBirth;

    bool m_wrap;
    bool m_useFirst;
    std::vector<bool> m_first;
    std::vector<bool> m_second;
};