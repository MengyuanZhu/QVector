#include "molecule.h"
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>
#include <QRegExp>
#include <QStringList>
#include <QDebug>
#include <QtCore/qmath.h>
#include <QString>
#include <QVector>
#include <QMessageBox>
Molecule::Molecule()
{
}





void Molecule::mol2_process( QString fileName )
{
    QString line;
    QFile	file( fileName );
    if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        //QMessageBox::warning( this, "Warnning", "No file name specified", QMessageBox::Yes );
        return;
    }

    QTextStream in( &file );
    i_molecule_number	= 0;
    line			= in.readLine();


    while ( !line.isNull() )
    {
        if ( line.indexOf( "@<TRIPOS>MOLECULE" ) >= 0 )
        {
            strMolname = in.readLine();
            i_molecule_number++;
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
                line = in.readLine();
            }
        }


        if ( line.indexOf( "@<TRIPOS>BOND" ) >= 0 )
        {
            line = in.readLine();
            while ( line.indexOf( " " ) >= 0 )                      /* read bond */
            {
                bondtemp = line.split( QRegExp( "\\s{1,}" ) );
                bond.append( bondtemp );
                line = in.readLine();
            }

            for ( i_h = atom.begin(); i_h != atom.end(); i_h++ )    /* analyse structure */
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
                                d_Vdistance.append( distance );
                                str_hydrogen_num1.append( i_h->at( 1 ) );
                                str_hydrogen_num2.append( j_h->at( 1 ) );

                                for ( i_bond = bond.begin(); i_bond != bond.end(); i_bond++ )
                                {
                                    if ( i_bond->at( 2 ) == i_h->at( 1 ) )
                                    {
                                        for ( i_atom_con = atom.begin(); i_atom_con != atom.end(); i_atom_con++ )
                                        {
                                            if ( i_atom_con->at( 1 ) == i_bond->at( 3 ) )
                                            {
                                                d_bond1.append( i_atom_con->at( 3 ).toDouble() - i_h->at( 3 ).toDouble() );
                                                d_bond1.append( i_atom_con->at( 4 ).toDouble() - i_h->at( 4 ).toDouble() );
                                                d_bond1.append( i_atom_con->at( 5 ).toDouble() - i_h->at( 5 ).toDouble() );
                                            }
                                        }
                                    }
                                    if ( i_bond->at( 3 ) == i_h->at( 1 ) )
                                    {
                                        for ( i_atom_con = atom.begin(); i_atom_con != atom.end(); i_atom_con++ )
                                        {
                                            if ( i_atom_con->at( 1 ) == i_bond->at( 2 ) )
                                            {
                                                d_bond1.append( i_atom_con->at( 3 ).toDouble() - i_h->at( 3 ).toDouble() );
                                                d_bond1.append( i_atom_con->at( 4 ).toDouble() - i_h->at( 4 ).toDouble() );
                                                d_bond1.append( i_atom_con->at( 5 ).toDouble() - i_h->at( 5 ).toDouble() );
                                            }
                                        }
                                    }
                                }


                                /*
                                 * ///////////////////////////////bond1 vector finished////////////////////////////
                                 * ///////////////////////////////bond2 vector//////////////////////////////////
                                 */
                                for ( i_bond = bond.begin(); i_bond != bond.end(); i_bond++ )
                                {
                                    if ( i_bond->at( 2 ) == j_h->at( 1 ) )
                                    {
                                        for ( i_atom_con = atom.begin(); i_atom_con != atom.end(); i_atom_con++ )
                                        {
                                            if ( i_atom_con->at( 1 ) == i_bond->at( 3 ) )
                                            {
                                                d_bond2.append( i_atom_con->at( 3 ).toDouble() - j_h->at( 3 ).toDouble() );
                                                d_bond2.append( i_atom_con->at( 4 ).toDouble() - j_h->at( 4 ).toDouble() );
                                                d_bond2.append( i_atom_con->at( 5 ).toDouble() - j_h->at( 5 ).toDouble() );
                                            }
                                        }
                                    }

                                    if ( i_bond->at( 3 ) == i_h->at( 1 ) )
                                    {
                                        for ( i_atom_con = atom.begin(); i_atom_con != atom.end(); i_atom_con++ )
                                        {
                                            if ( i_atom_con->at( 1 ) == i_bond->at( 2 ) )
                                            {
                                                d_bond2.append( i_atom_con->at( 3 ).toDouble() - j_h->at( 3 ).toDouble() );
                                                d_bond2.append( i_atom_con->at( 4 ).toDouble() - j_h->at( 4 ).toDouble() );
                                                d_bond2.append( i_atom_con->at( 5 ).toDouble() - j_h->at( 5 ).toDouble() );
                                            }
                                        }
                                    }
                                }
                                /* ///////////////////////////////bond2 vector finished//////////////////////////// */
                                angle	= qAcos( (d_bond1.at( 0 ) * d_bond2.at( 0 ) + d_bond1.at( 1 ) * d_bond2.at( 1 ) + d_bond1.at( 2 ) * d_bond2.at( 2 ) ) / (sqrt( d_bond1.at( 0 ) * d_bond1.at( 0 ) + d_bond1.at( 1 ) * d_bond1.at( 1 ) + d_bond1.at( 2 ) * d_bond1.at( 2 ) ) * sqrt( d_bond2.at( 0 ) * d_bond2.at( 0 ) + d_bond2.at( 1 ) * d_bond2.at( 1 ) + d_bond2.at( 2 ) * d_bond2.at( 2 ) ) ) );
                                angle	= angle / 3.141592654 * 180;
                                d_Vangle.append( angle );
                                d_bond1.clear();
                                d_bond2.clear();



                        }
                    }
                }
            }

            if ( line.indexOf( " " ) < 0 ) /* finish bond */
            {

                score_process();

                continue;
            }
        }

        line = in.readLine();
        ui->statusBar->showMessage( QString::number( i_molecule_number ) + " molecules processed" );


        QCoreApplication::processEvents();
        if ( this->i_flag_stop == 1 )
        {
            break;
            file.close();
        }
    }
    if ( this->i_flag_stop == 1 )
    {
        ui->statusBar->showMessage( "Screening Stopped" );
    }else {
        ui->statusBar->showMessage( "Screening Finished" );
    }
    ui->tb_result->sortByColumn( 1 );
    if ( ui->tb_result->rowCount() > ui->le_hit->text().toInt() )
    {
        for (; i_hit_number >= ui->le_hit->text().toInt(); i_hit_number-- )
        {
            ui->tb_result->removeRow( i_hit_number );
        }
    }
    file.close();
}

