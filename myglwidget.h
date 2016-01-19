/******************************************************************************
* Copyright 2014 Department of Chemistry, Georgia State University
*
* File    :    myglwidget.h
*
* Author  :    Mengyuan Zhu (mzhu7@gsu.edu)
*
* Date    :    5/24/2014
*
* Version :    1.0
*
* This file is part of the QVector program.
*
* The QVector is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 3 of the
* License, or (at your option) any later version.
*
* The QVector is distributed in the hope that it will be
* useful, but WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General
* Public License along with this program.  If not, see
* <http://www.gnu.org/licenses/>.
*
******************************************************************************/

#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H
#include "mainwindow.h"
#include <QGLWidget>
#include <GL/glu.h>
class MyGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit MyGLWidget(QWidget *parent = 0);
    ~MyGLWidget();

    QString m_filename;
    QString m_hydrogen1;
    QString m_hydrogen2;


protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void draw();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void paintGL();
    void renderCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius, GLUquadricObj *quadric);
    void drawatom(GLfloat color[3]);
    void setLightColor(GLfloat light_color[3]);


private:
    GLfloat rotationX;
    GLfloat rotationY;
    GLfloat rotationZ;
    GLfloat translateX;
    GLfloat translateY;

    QColor faceColors[4];
    QPoint lastPos;
    GLfloat resize_ratio;
    GLdouble sphereRadius;
    GLdouble cylinderRadius;
};


#endif // MYGLWIDGET_H
