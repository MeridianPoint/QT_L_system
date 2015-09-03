#ifndef TRACKBALL_H
#define TRACKBALL_H
#include "stable.h"

class TrackBall
{
private:
    Vec3f StartPosition;
    Vec3f curPosition;
    int Window_width;
    int Window_height;
    int curx, cury;
    int startX, startY;
    Vec3f axis, position;
    float angle = 0.0;

    bool trackingMouse = false;
    bool forceRedraw = false;
    bool trackballMoved = false;

    Vec3f LastPos = Vec3f(0.0, 0.0, 0.0);
public:
    TrackBall();
    TrackBall(int width,int height);
    ~TrackBall();

    void TrackBall::StartRotation(int x, int y);
    void TrackBall::StopRotation(int x, int y);
    void trackball_ptov(int x, int y, int width, int height, Vec3f &v);
};

#endif // TRACKBALL_H
