/******************************************************************************
* Copyright 2014 Department of Chemistry, Georgia State University
*
* File    :    mainwindow.cpp
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
#include "ui_mainwindow.h"
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>
#include <QRegExp>
#include <QStringList>
#include <QDebug>
#include <QtCore/qmath.h>
#include <QtSql>

MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    ui->setupUi( this );
    ui->statusBar->showMessage( "Status" );
    ui->pb_stop->setEnabled( false );
    ui->le_distance_factor->setValidator( new QDoubleValidator( 0, 1, 10, this ) );
    ui->le_angle_factor->setValidator( new QDoubleValidator( 0, 1, 10, this ) );
    ui->tb_result->setEnabled(false);
    //ui->tb_result->hideColumn(2);
    //ui->tb_result->hideColumn(3);

    i_flag_stop	= 0;
    b_connect_atom1 = false;
    b_connect_atom2 = false;


    QSqlDatabase database = QSqlDatabase::addDatabase( "QSQLITE" );
    database.setDatabaseName( "database.db" );
    if ( !database.open() )
    {
        QMessageBox::information( this, tr( "Connection..." ), tr( "Connetion error!" ) );
        return;
    }
    QSqlQuery query;
    query.prepare( "CREATE TABLE IF NOT EXISTS molecule (name VARCHAR(30),content TEXT)" );
    query.exec();
}


MainWindow::~MainWindow()
{
    //QFile::remove( "database.db" );
    QFile::remove("molecule.mol2");
    delete ui;
}


void MainWindow::on_pb_submit_clicked()
{

    for ( int i = ui->tb_result->rowCount() - 1; i >= 0; i-- ) /* remove table contents */
    {
        ui->tb_result->removeRow( i );
    }
    ui->tb_result->insertRow(0);

    ui->tb_result->setEnabled( false );
    ui->pb_stop->setEnabled( true );

    this->i_flag_stop	= 0;
    d_inputdistance		= ui->le_distance->text().toDouble();
    d_inputdistance_std	= ui->le_distance_std->text().toDouble();
    d_inputangle		= ui->le_angle->text().toDouble();
    d_inputangle_std	= ui->le_angle_std->text().toDouble();
    i_hit_number		= 0;
    i_molecule_number  =0;
    molecule_weight     = 0;
    atom_number=0;
    bond_number=0;

    connectatom.clear();
    if ( ui->cb_carbon->isChecked() )
    {
        connectatom.append( "C" );
    }
    if ( ui->cb_nitrogen->isChecked() )
    {
        connectatom.append( "N" );
    }
    if ( ui->cb_oxygen->isChecked() )
    {
        connectatom.append( "O" );
    }


    if ( d_inputdistance == 0 )
    {
        QMessageBox::warning( NULL, "warning", "Please input distance!" );
        return;
    }

    if (ui->cb_format->currentIndex()==0)
    {
        process_mol2();
    }
    else
    {
        process_sdf();
    }



    if ( this->i_flag_stop == 1 )
    {
        ui->statusBar->showMessage( "Screening Stopped" );
    }else {
        ui->statusBar->showMessage( "Screening Finished" );
    }

    ui->tb_result->sortByColumn( 1 );

    int temp = i_hit_number;
    if ( ui->tb_result->rowCount() > ui->le_hit->text().toInt() )
    {
        for (; i_hit_number >= ui->le_hit->text().toInt(); i_hit_number-- )
        {
            ui->tb_result->removeRow( i_hit_number );
        }
    }
    i_hit_number = temp;
}

void MainWindow::process_sdf()
{

}

void MainWindow::process_mol2()
{
    QSqlDatabase	database = QSqlDatabase::database();
    QSqlQuery	query;
    database.transaction();

    QString fileName = ui->le_db->text();
    QFile	file( fileName );
    if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        QMessageBox::warning( this, "Warnning", "No file name specified", QMessageBox::Yes );
        return;
    }
    QTextStream	in( &file );
    QString		line;
    line = in.readLine();

    while ( !line.isNull() )
    {
        if ( line.indexOf( "@<TRIPOS>MOLECULE" ) >= 0 )
        {
            strMolname = in.readLine();
            i_molecule_number++;
            atom.clear();
            bond.clear();
            molecule_weight=0;
        }

        if ( line.indexOf( "@<TRIPOS>ATOM" ) >= 0 )
        {
            line			= in.readLine();
            molecule_content	+= "@<TRIPOS>ATOM\n";

            while ( line.indexOf( "@<TRIPOS>BOND" ) < 0 )
            {
                atomtemp		= line.split( QRegExp( "\\s{1,}" ) );
                atomname	= atomtemp.at( 2 );
                atomname	= atomname.remove( QRegExp( "\\d*" ) );

                if (atomname=="C")
                {
                    molecule_weight+=12.011;
                }
                else if (atomname=="N")
                {
                   molecule_weight+=14.007;
                }
                else if (atomname=="H")
                {
                    molecule_weight+=1.0079;
                }
                else if (atomname=="O")
                {
                    molecule_weight+=15.999;
                }
                else if (atomname=="B")
                {
                    molecule_weight+=10.811;
                }
                else if (atomname=="S")
                {
                    molecule_weight+=32.065;
                }
                else if (atomname=="P")
                {
                    molecule_weight+=30.974;
                }
                else if (atomname=="F")
                {
                    molecule_weight+=18.998;
                }
                else if (atomname=="Cl")
                {
                    molecule_weight+=35.453;
                }
                else if (atomname=="Br")
                {
                   molecule_weight+=79.904;
                }
                else if (atomname=="I")
                {
                   molecule_weight+=126.90;
                }

                molecule_content	= molecule_content + line + "\n";
                atom.append( atomtemp );
                atom_number++;
                line = in.readLine();
            }
        }


        if ( line.indexOf( "@<TRIPOS>BOND" ) >= 0 )
        {
            line			= in.readLine();
            molecule_content	+= "@<TRIPOS>BOND\n";

            while ( line.indexOf( " " ) >= 0 ) /* read bond */
            {
                bondtemp		= line.split( QRegExp( "\\s{1,}" ) );
                molecule_content	= molecule_content + line + "\n";
                bond.append( bondtemp );
                bond_number++;
                line = in.readLine();
            }


            for ( i_h = atom.begin(); i_h != atom.end(); i_h++ ) /* analyse structure */
            {
                if ( i_h->at( 2 ).indexOf( "H" ) >= 0 )
                {
                    for ( j_h = i_h + 1; j_h != atom.end(); j_h++ )
                    {
                        if ( j_h->at( 2 ).indexOf( "H" ) >= 0 )
                        {
                            distance	= qPow( (i_h->at( 3 ).toDouble() - j_h->at( 3 ).toDouble() ), 2 ) + qPow( (i_h->at( 4 ).toDouble() - j_h->at( 4 ).toDouble() ), 2 ) + qPow( (i_h->at( 5 ).toDouble() - j_h->at( 5 ).toDouble() ), 2 );
                            distance	= qSqrt( distance );
                            if ( (distance >= (d_inputdistance - d_inputdistance_std) ) & (distance <= (d_inputdistance + d_inputdistance_std) ) )
                            {
                                /* ///////////////////////////////bond1 vector////////////////////////////////// */

                                for ( i_bond = bond.begin(); i_bond != bond.end(); i_bond++ )
                                {
                                    if ( i_bond->at( 2 ) == i_h->at( 1 ) )
                                    {
                                        for ( i_atom_con = atom.begin(); i_atom_con != atom.end(); i_atom_con++ )
                                        {
                                            atomname	= i_atom_con->at( 2 );
                                            atomname	= atomname.remove( QRegExp( "\\d*" ) );

                                            if ( (i_atom_con->at( 1 ) == i_bond->at( 3 ) ) && (connectatom.contains( atomname ) ) )
                                            {
                                                str_hydrogen_num1.append( i_h->at( 1 ) );
                                                d_bond1.append( i_atom_con->at( 3 ).toDouble() - i_h->at( 3 ).toDouble() );
                                                d_bond1.append( i_atom_con->at( 4 ).toDouble() - i_h->at( 4 ).toDouble() );
                                                d_bond1.append( i_atom_con->at( 5 ).toDouble() - i_h->at( 5 ).toDouble() );
                                                b_connect_atom1 = true;
                                                break;
                                            }
                                        }
                                        break;
                                    }
                                    if ( i_bond->at( 3 ) == i_h->at( 1 ) )
                                    {
                                        for ( i_atom_con = atom.begin(); i_atom_con != atom.end(); i_atom_con++ )
                                        {
                                            atomname	= i_atom_con->at( 2 );
                                            atomname	= atomname.remove( QRegExp( "\\d*" ) );

                                            if ( (i_atom_con->at( 1 ) == i_bond->at( 2 ) ) && (connectatom.contains( atomname ) ) )
                                            {
                                                str_hydrogen_num1.append( i_h->at( 1 ) );
                                                d_bond1.append( i_atom_con->at( 3 ).toDouble() - i_h->at( 3 ).toDouble() );
                                                d_bond1.append( i_atom_con->at( 4 ).toDouble() - i_h->at( 4 ).toDouble() );
                                                d_bond1.append( i_atom_con->at( 5 ).toDouble() - i_h->at( 5 ).toDouble() );
                                                b_connect_atom1 = true;
                                                break;
                                            }
                                        }
                                        break;
                                    }
                                }
                                /*///////////////////////////bond1 vector finished//////////////////////////// */



                                /* ///////////////////////////////bond2 vector//////////////////////////////////
                                 */
                                for ( i_bond = bond.begin(); i_bond != bond.end(); i_bond++ )
                                {
                                    if ( i_bond->at( 2 ) == j_h->at( 1 ) )
                                    {
                                        for ( i_atom_con = atom.begin(); i_atom_con != atom.end(); i_atom_con++ )
                                        {
                                            atomname	= i_atom_con->at( 2 );
                                            atomname	= atomname.remove( QRegExp( "\\d*" ) );
                                            if ( (i_atom_con->at( 1 ) == i_bond->at( 3 ) ) && (connectatom.contains( atomname ) ) )
                                            {
                                                str_hydrogen_num2.append( j_h->at( 1 ) );
                                                d_bond2.append( i_atom_con->at( 3 ).toDouble() - j_h->at( 3 ).toDouble() );
                                                d_bond2.append( i_atom_con->at( 4 ).toDouble() - j_h->at( 4 ).toDouble() );
                                                d_bond2.append( i_atom_con->at( 5 ).toDouble() - j_h->at( 5 ).toDouble() );
                                                b_connect_atom2 = true;
                                                break;
                                            }
                                        }
                                        break;
                                    }

                                    if ( i_bond->at( 3 ) == j_h->at( 1 ) )
                                    {
                                        for ( i_atom_con = atom.begin(); i_atom_con != atom.end(); i_atom_con++ )
                                        {
                                            atomname	= i_atom_con->at( 2 );
                                            atomname	= atomname.remove( QRegExp( "\\d*" ) );
                                            if ( (i_atom_con->at( 1 ) == i_bond->at( 2 ) ) && (b_connect_atom1) && (connectatom.contains( atomname ) ) )
                                            {
                                                str_hydrogen_num2.append( j_h->at( 1 ) );
                                                d_bond2.append( i_atom_con->at( 3 ).toDouble() - j_h->at( 3 ).toDouble() );
                                                d_bond2.append( i_atom_con->at( 4 ).toDouble() - j_h->at( 4 ).toDouble() );
                                                d_bond2.append( i_atom_con->at( 5 ).toDouble() - j_h->at( 5 ).toDouble() );
                                                b_connect_atom2 = true;
                                                break;
                                            }
                                        }
                                        break;
                                    }
                                }
                                /* ///////////////////////////////bond2 vector finished//////////////////////////// */
                                angle	= qAcos( (d_bond1.at( 0 ) * d_bond2.at( 0 ) + d_bond1.at( 1 ) * d_bond2.at( 1 ) + d_bond1.at( 2 ) * d_bond2.at( 2 ) ) / (sqrt( d_bond1.at( 0 ) * d_bond1.at( 0 ) + d_bond1.at( 1 ) * d_bond1.at( 1 ) + d_bond1.at( 2 ) * d_bond1.at( 2 ) ) * sqrt( d_bond2.at( 0 ) * d_bond2.at( 0 ) + d_bond2.at( 1 ) * d_bond2.at( 1 ) + d_bond2.at( 2 ) * d_bond2.at( 2 ) ) ) );
                                angle	= angle / 3.141592654 * 180;

                                d_bond1.clear();
                                d_bond2.clear();
                                if ( b_connect_atom1 && b_connect_atom2 )
                                {
                                    d_score = (1 - qAbs( d_inputdistance - distance ) / d_inputdistance_std) * ui->le_distance_factor->text().toDouble() + (1 - qAbs( d_inputangle - angle ) / d_inputangle_std) * ui->le_angle_factor->text().toDouble();
                                    d_Vscore.append( d_score );
                                }
                                b_connect_atom1 = false;
                                b_connect_atom2 = false;
                            }
                        }
                    }
                }
            }


            if ( line.indexOf( " " ) < 0 ) /* finish bond */
            {
                if ( !d_Vscore.empty() )
                {
                    QVector<double>::iterator	i_maxscore;
                    double				d_maxscore	= 0;
                    int				i_numofhydrogen = 0;
                    for ( i_maxscore = d_Vscore.begin(); i_maxscore != d_Vscore.end(); i_maxscore++ )
                    {
                        if ( *i_maxscore > d_maxscore )
                        {
                            d_maxscore = *i_maxscore;
                            i_numofhydrogen++;
                        }
                    }

                    if ( (d_maxscore > 0 ) && (molecule_weight<=ui->le_weight->text().toDouble()))
                    {
                        ui->tb_result->insertRow( ui->tb_result->rowCount() );
                        ui->tb_result->setItem( i_hit_number, 0, new QTableWidgetItem( strMolname ) );
                        ui->tb_result->setItem( i_hit_number, 1, new QTableWidgetItem( QString::number( d_maxscore ) ) );
                        ui->tb_result->setItem( i_hit_number, 2, new QTableWidgetItem( str_hydrogen_num1.at( i_numofhydrogen - 1 ) ) );
                        ui->tb_result->setItem( i_hit_number, 3, new QTableWidgetItem( str_hydrogen_num2.at( i_numofhydrogen - 1 ) ) );
                        ui->tb_result->setItem( i_hit_number, 4, new QTableWidgetItem( QString::number(molecule_weight,'f',6 ) ));
                        sql = QString( "insert into molecule values (\'%1\',\' %2\')" ).arg( strMolname ).arg( molecule_content );
                        query.prepare( sql );
                        query.exec();
                        i_hit_number++;
                    }
                }
                atom.clear();
                bond.clear();


                d_Vscore.clear();
                str_hydrogen_num1.clear();
                str_hydrogen_num2.clear();
                molecule_content = "";
                atom_number=0;
                bond_number=0;

                continue;
            }
        }

        line = in.readLine();
        ui->statusBar->showMessage( QString::number( i_molecule_number ) + " molecules processed" );
        QCoreApplication::processEvents();
        if ( this->i_flag_stop == 1 )
        {
            database.commit();
            file.close();
            break;
        }
    }
    database.commit();
    file.close();
    ui->pb_stop->setEnabled(false);
    ui->tb_result->setEnabled( true );
}


void MainWindow::on_pb_browse_clicked()
{
    QString fileName = QFileDialog::getOpenFileName( this, tr( "Open FIle" ), ".", tr( "Tripos Mol2 Files(*.mol2)" ) );
    ui->le_db->setText( fileName );
}


void MainWindow::on_pb_stop_clicked()
{
    this->i_flag_stop = 1;
    ui->pb_stop->setEnabled( false );
}


void MainWindow::on_le_distance_factor_editingFinished()
{
    ui->le_angle_factor->setText( QString::number( 1 - ui->le_distance_factor->text().toDouble() ) );
}


void MainWindow::on_le_angle_factor_editingFinished()
{
    ui->le_distance_factor->setText( QString::number( 1 - ui->le_angle_factor->text().toDouble() ) );
}


void MainWindow::on_actionMolecular_View_triggered()
{
    if ( ui->actionMolecular_View->text() == "Show Molecular Viewer" )
    {
        ui->Dock_MolView->show();
    }else {
        ui->Dock_MolView->hide();
    }
}


void MainWindow::on_Dock_MolView_visibilityChanged( bool visible )
{
    if ( visible == true )
    {
        ui->actionMolecular_View->setText( "Hide Molecular Viewer" );
    }else {
        ui->actionMolecular_View->setText( "Show Molecular Viewer" );
    }
}


void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_actionAbout_triggered()
{
    QApplication::aboutQt();
}


void MainWindow::on_actionAbout_2_triggered()
{
    QMessageBox message( QMessageBox::NoIcon, "About QVector", "QVecotor: a program for screening fragments.\n\nDevelopted by Mengyuan Zhu\nEmail: mzhu7@gsu.edu.\n\n" );
    message.setIconPixmap( QPixmap( ":/source/icon.png" ).scaledToHeight( 100 ) );
    message.exec();
}


void MainWindow::on_tb_result_clicked( const QModelIndex &index )
{
    if ( index.row() > i_hit_number - 1 )
        return;
    this->on_tb_result_currentCellChanged(index.row(),0,0,0);
}


void MainWindow::on_actionExport_to_CSV_file_triggered()
{
    QString		fileName = QFileDialog::getSaveFileName( this, tr( "Save File" ), ".", tr( "CSV File (*.csv)" ) );
    QFile		file( fileName );
    QTextStream	out( &file );
    QString		str_itemtext;

    if ( file.open( QFile::WriteOnly ) )
    {
        for ( int column = 0; column < ui->tb_result->columnCount(); ++column )
        {
            out << ui->tb_result->horizontalHeaderItem( column )->data( Qt::DisplayRole ).toString() + ", ";
        }
        out << "\n";

        for ( int row = 0; row < (ui->tb_result->rowCount() - 1); ++row )
        {
            for ( int column = 0; column < (ui->tb_result->columnCount() ); ++column )
            {
                str_itemtext = ui->tb_result->item( row, column )->text();
                out << str_itemtext + ",";
            }
            out << "\n";
        }
        file.close();
    }
}

void MainWindow::on_tb_result_currentCellChanged(int currentRow,int ,int ,int )
{
    if ( currentRow > i_hit_number - 1 )
        return;

    QSqlDatabase	database = QSqlDatabase::database();
    QSqlQuery	query;
    QString		sql = QString( "SELECT content FROM molecule where name=\'%1\'" ).arg( ui->tb_result->item( currentRow, 0 )->text() );
    query.prepare( sql );
    query.exec();
    query.next();

    QFile file( "molecule.mol2" );
    if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
    {
        QMessageBox::warning( this, "Warnning", "No file name specified", QMessageBox::Yes );
        return;
    }

    QTextStream out( &file );
    out << query.value( 0 ).toString();

    ui->widget_molecule->m_filename		= "molecule.mol2";
    ui->widget_molecule->m_hydrogen1	= ui->tb_result->item( currentRow, 2 )->text();
    ui->widget_molecule->m_hydrogen2	= ui->tb_result->item( currentRow, 3 )->text();
    ui->widget_molecule->update();
    database.close();

}
