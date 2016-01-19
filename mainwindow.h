/******************************************************************************
* Copyright 2014 Department of Chemistry, Georgia State University
*
* File    :    mainwindow.h
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_pb_submit_clicked();
    void on_pb_browse_clicked();
    void on_pb_stop_clicked();
    void on_le_distance_factor_editingFinished();
    void on_le_angle_factor_editingFinished();
    void on_actionMolecular_View_triggered();
    void on_Dock_MolView_visibilityChanged(bool visible);
    void on_actionExit_triggered();
    void on_actionAbout_triggered();
    void on_actionAbout_2_triggered();
    void on_tb_result_clicked(const QModelIndex &index);
    void on_actionExport_to_CSV_file_triggered();
    void process_mol2();
    void process_sdf();
    void on_tb_result_currentCellChanged(int currentRow, int, int, int);

private:

    double    d_inputdistance; //input
    double    d_inputdistance_std; //input
    double    d_inputangle; //input
    double    d_inputangle_std; //input
    int	i_molecule_number;
    int i_flag_stop;
    int i_hit_number;
    QString sql;
    QStringList connectatom; //C,N,O selection
    Ui::MainWindow *ui;

    QString		strMolname;               //Parse database
    QVector<QStringList>		atom;
    QVector<QStringList>		bond;
    QStringList			atomtemp;
    QStringList			bondtemp;
    QVector<QStringList>::iterator	i_h;
    QVector<QStringList>::iterator	j_h;
    QString molecule_content;
    double molecule_weight;
    int atom_number;
    int bond_number;

    double				distance;
    double				angle;
    double				d_score;
    QVector<double>			d_Vscore;
    QVector<QStringList>::iterator	i_bond;
    QVector<QStringList>::iterator	i_atom_con;
    QVector<double>			d_bond1;
    QVector<double>			d_bond2;
    QVector<QString> str_hydrogen_num1;
    QVector<QString> str_hydrogen_num2;


    bool b_connect_atom1;
    bool b_connect_atom2;
    QString atomname;
};

#endif // MAINWINDOW_H
