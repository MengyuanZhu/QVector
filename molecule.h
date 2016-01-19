#ifndef MOLECULE_H
#define MOLECULE_H
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>
#include <QRegExp>
#include <QStringList>
#include <QDebug>
#include <QtCore/qmath.h>
class Molecule
{
public:
    Molecule();

    void mol2_process(QString filename);


    double	d_inputdistance;
    double	d_inputdistance_std;
    double	d_inputangle;
    double	d_inputangle_std;
    int i_hit_number;
    QString		strMolname;
    QVector<QStringList>		atom;
    QVector<QStringList>		bond;
    QStringList			atomtemp;
    QStringList			bondtemp;
    QVector<QStringList>::iterator	i_h;
    QVector<QStringList>::iterator	j_h;
    double				distance;
    double				angle;
    double				d_score;
    QVector<double>         d_Vdistance;
    QVector<double>         d_Vangle;
    QVector<double>			d_Vscore;
    QVector<QStringList>::iterator	i_bond;
    QVector<QStringList>::iterator	i_atom_con;
    QVector<double>			d_bond1;
    QVector<double>			d_bond2;
    QVector<QString> str_hydrogen_num1;
    QVector<QString> str_hydrogen_num2;
    int	i_molecule_number; //number of molecule processed.
    int i_flag_stop;       //stop button click flag.
    int i_flag_showmolecule; //if 1, show molecule; if 0, show score.
    int i_checkbox;

};

#endif // MOLECULE_H
