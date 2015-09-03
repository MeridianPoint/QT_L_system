#include "trackball.h"

TrackBall::TrackBall()
{

}

TrackBall::~TrackBall()
{

}

void TrackBall::StartRotation(int x, int y){
    trackingMouse = true;
    forceRedraw = false;
    startX = x;
    startY = y;
    curx = x;
    cury = y;
    trackball_ptov(x, y, Window_width, Window_height, LastPos);
    trackballMoved = true;
}

void TrackBall::StopRotation(int x, int y){
    trackingMouse = false;
    /* check if position has changed */
    if (startX != x || startY != y)
        forceRedraw = true;
    else
    {
        angle = 0.0;
        forceRedraw = false;
        trackballMoved = false;
    }
}

void TrackBall::trackball_ptov(int x, int y, int width, int height, Vec3f &v)
{
    float d, a;
    /* project x,y onto a hemisphere centered within width, height ,
    note z is up here*/
    float v0 = (2.0*x - width) / width;
    float v1 = (height - 2.0F*y) / height;
    d = sqrt(v0 * v0 + v1 * v1);
    v = Vec3f(v0, v1, cos((PI / 2.0) * ((d < 40.0) ? d : 40.0)));

    a = 1.0 / v.length();
    v *= a;
    v.normalize();
}
