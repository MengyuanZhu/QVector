/******************************************************************************
* Copyright 2014 Department of Chemistry, Georgia State University
*
* File    :    myglwidget.cpp
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


#include "myglwidget.h"
#include <QtGui>
#include <QtOpenGL>
#include <GL/glu.h>
#include <QDebug>
#include <QRegExp>

///////////////atom color//////////////
GLfloat hydrogen[3] =	{1.0, 1.0, 1.0};
GLfloat boron[3] =	    {1.0, 0.7, 0.7};
GLfloat carbon[3] =		{0.5, 0.5, 0.5};
GLfloat nitrogen[3] =	{0.0, 0.0, 1.0};
GLfloat oxygen[3] =		{1.0, 0.0, 0.0};
GLfloat phosphate[3] =	{1.0, 0.5, 0.0};
GLfloat sulfur[3] =	    {0.9, 0.8, 0.3};
GLfloat fluorine[3] =	{0.7, 1.0, 1.0};
GLfloat chloride[3] =	{0.1, 0.9, 0.1};
GLfloat bromine[3] =	{0.7, 0.2, 0.2};
GLfloat iodine[3] =	    {0.6, 0.0, 0.6};
GLfloat gray[3] =	    {0.5, 0.5, 0.5};
///////////////atom color//////////////



void MyGLWidget::setLightColor(GLfloat light_color[3])
{
    GLfloat mat_specular[]={ 1.0, 1.0, 1.0, 1.0 };
    GLfloat shine[] = {100.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular );
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_color );
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color );
    glMaterialfv(GL_FRONT, GL_SHININESS, shine);
}

MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glFlush();
    resize_ratio=0.2;
    sphereRadius = 0.25;
    cylinderRadius = 0.05;
}

MyGLWidget::~MyGLWidget()
{
}

void MyGLWidget::initializeGL()
{
    qglClearColor(Qt::black);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void MyGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat x = GLfloat(width) / height;
    glFrustum(-x, +x, -1.0, +1.0, 4.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
}

void MyGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw();
}

void MyGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    GLfloat dx = GLfloat(event->x() - lastPos.x()) / width();
    GLfloat dy = GLfloat(event->y() - lastPos.y()) / height();

    if (event->buttons() & Qt::LeftButton) {
        rotationX += 180 * dy;
        rotationY += 180 * dx;
        updateGL();
    } else if (event->buttons() & Qt::RightButton) {
        translateX +=  3.5*dx;
        translateY -=  3.5*dy;
        updateGL();
    }
    lastPos = event->pos();
}

void MyGLWidget::wheelEvent(QWheelEvent *event)
{
    int step=event->delta()/120;

    if (step>=1){
        resize_ratio=resize_ratio*1.1;
        updateGL();

    }
    else{
        resize_ratio=resize_ratio*0.90;
        updateGL();

    }
}

void MyGLWidget::drawatom(GLfloat color[3])
{
    glColor3fv(color);
    setLightColor(color);
    GLUquadric *myQuad;
    myQuad=gluNewQuadric();
    gluSphere(myQuad , sphereRadius , 100 , 100);
    sphereRadius=0.25;
}

void MyGLWidget::renderCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius, GLUquadricObj *quadric)
{
    setLightColor(gray);
    float vx = x2-x1;
    float vy = y2-y1;
    float vz = z2-z1;
    float ax,rx,ry,rz;
    float len = sqrt( vx*vx + vy*vy + vz*vz );

    glPushMatrix();
    glTranslatef( x1,y1,z1 );
    if (fabs(vz) < 0.0001)
    {
        glRotatef(90, 0,1,0);
        ax = 57.2957795*-atan( vy / vx );
        if (vx < 0)
        {
            ax = ax + 180;
        }
        rx = 1;
        ry = 0;
        rz = 0;
    }
    else
    {
        ax = 57.2957795*acos( vz/ len );
        if (vz < 0.0)
            ax = -ax;
        rx = -vy*vz;
        ry = vx*vz;
        rz = 0;
    }
    glRotatef(ax, rx, ry, rz);
    gluQuadricOrientation(quadric,GLU_OUTSIDE);
    gluCylinder(quadric, radius, radius, len, 10, 10);
    glPopMatrix();
}

void MyGLWidget::draw()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(rotationX, 10.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 10.0, 0.0);
    glRotatef(rotationZ, 0.0, 0.0, 10.0);
    glTranslatef(translateX,translateY,0);
    glScalef(resize_ratio,resize_ratio,resize_ratio);
    glLineWidth(2.5);

    if (m_filename=="")
    {
        return;
    }

    QString fileName = m_filename;
    QString line;
    QFile	file( fileName );
    if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        QMessageBox::warning( NULL, "Warnning", "No file name specified", QMessageBox::Yes );
        return;
    }
    QTextStream	in( &file );
    QString		strMolname;
    line = in.readLine();
    QVector<QStringList>		atom;
    QVector<QStringList>		bond;
    QStringList			atomtemp;
    QStringList			bondtemp;


    while ( !line.isNull() )
    {
        if ( line.indexOf( "@<TRIPOS>MOLECULE" ) >= 0 )
        {
            strMolname = in.readLine();
            atom.clear();
            bond.clear();
        }

        if ( line.indexOf( "@<TRIPOS>ATOM" ) >= 0 )
        {
            line = in.readLine();
            while ( line.indexOf( "@<TRIPOS>BOND" ) < 0 )
            {
                atomtemp = line.split( QRegExp( "\\s{1,}" ) );
                atom.append( atomtemp );
                QString atomname=atomtemp.at(2);
                atomname = atomname.remove( QRegExp("\\d*"));


                glPushMatrix();
                glTranslatef(atomtemp.at(3).toDouble(),atomtemp.at(4).toDouble(), atomtemp.at(5).toDouble());
                if (atomname=="C")
                {
                        drawatom(carbon);
                }
                else if(atomname=="N")
                {
                    drawatom(nitrogen);
                }
                else if(atomname=="H")
                {
                    if (atomtemp.at(1)==m_hydrogen1 or atomtemp.at(1)==m_hydrogen2)
                    {
                    sphereRadius=0.5;
                    }
                    drawatom(hydrogen);
                }
                else if(atomname=="O")
                {
                    drawatom(oxygen);
                }
                else if(atomname=="F")
                {
                    drawatom(fluorine);
                }
                else if(atomname=="Cl")
                {
                    drawatom(chloride);
                }
                else if(atomname=="Br")
                {
                    drawatom(bromine);
                }
                else if(atomname=="I")
                {
                    drawatom(iodine);
                }
                else if(atomname=="B")
                {
                    drawatom(boron);
                }
                else if(atomname=="S")
                {
                    drawatom(sulfur);
                }
                else if(atomname=="P")
                {
                    drawatom(phosphate);
                }
                glPopMatrix();

                line = in.readLine();
            }
        }


        if ( line.indexOf( "@<TRIPOS>BOND" ) >= 0 )
        {
            glPushMatrix();
            GLUquadric *myQuad;
            myQuad=gluNewQuadric();

            line = in.readLine();
            while ( line.indexOf( " " ) >= 0 )                      /* read bond */
            {
                bondtemp = line.split( QRegExp( "\\s{1,}" ) );
                bond.append( bondtemp );
                renderCylinder(atom[bondtemp.at(2).toInt()-1].at(3).toFloat(), atom[bondtemp.at(2).toInt()-1].at(4).toFloat(), atom[bondtemp.at(2).toInt()-1].at(5).toFloat(), atom[bondtemp.at(3).toInt()-1].at(3).toFloat(), atom[bondtemp.at(3).toInt()-1].at(4).toFloat(), atom[bondtemp.at(3).toInt()-1].at(5).toFloat(), cylinderRadius, myQuad);
                line = in.readLine();
            }

            glPopMatrix();

            if ( line.indexOf( " " ) < 0 ) /* finish bond */
            {
                continue;
            }
        }
        line = in.readLine();
        }
}
