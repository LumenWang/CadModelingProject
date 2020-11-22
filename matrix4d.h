/* Õº–ŒÕºœÒ…Ëº∆ */
#ifndef MATRIX_H
#define MATRIX_H
#include <QVector3D>

class matrix4d
{
public:
    matrix4d();
    matrix4d(QVector3D axis,double angle);
    QVector3D dotProduct(QVector3D &T);
    double value[4][4];
};

#endif // MATRIX_H
