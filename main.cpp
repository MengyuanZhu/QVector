/******************************************************************************
* Copyright 2014 Department of Chemistry, Georgia State University
*
* File    :    main.cpp
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
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
