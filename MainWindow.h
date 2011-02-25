/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is
 * 'psimpl - generic n-dimensional polyline simplification'.
 *
 * The Initial Developer of the Original Code is
 * Elmar de Koning.
 * Portions created by the Initial Developer are Copyright (C) 2010-2011
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * ***** END LICENSE BLOCK ***** */

/*!
    psimpl - generic n-dimensional polyline simplification
    Copyright (C) 2010-2011 Elmar de Koning, edekoning@gmail.com

    This file is part of psimpl, and is hosted at SourceForge:
    http://sourceforge.net/projects/psimpl/ Originally psimpl was released
    as part of the article 'Polyline Simplification' at The Code Project:
    http://www.codeproject.com/KB/recipes/PolylineSimplification.aspx

    28-09-2010 - Initial version
    23-10-2010 - Changed license from CPOL to MPL
    26-10-2010 - Clarified input (type) requirements, and changed the
                 behavior of the algorithms under invalid input
    01-12-2010 - Added the nth point, perpendicular distance and Reumann-Witkam routines;
                 moved all functions related to distance calculations to the math namespace
    10-12-2010 - Fixed a bug in the perpendicular distance routine
    24-02-2010 - Added Opheim simplification, and functions for computing positional errors
                 due to simplification; renamed simplify_douglas_peucker_alt to
                 simplify_douglas_peucker_n
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QtGui/QMainWindow>


namespace Ui {
    class MainWindow;
}


namespace psimpl {

    class DPWorker;

    typedef enum ALGORITHM
    {
        NTH_POINT,
        RADIAL_DISTANCE,
        PERPENDICULAR_DISTANCE,
        REUMANN_WITKAM,
        OPHEIM,
        //LANG,
        DOUGLAS_PEUCKER,
        DOUGLAS_PEUCKER_VARIANT,
        DOUGLAS_PEUCKER_REFERENCE,
    } Algorithm;

    /*!
        \brief Mainwindow where polylines can be generated and simplified.
        
        Multiple simplification algorithms, implementations and container types can be experimented
        with.
    */
    class MainWindow : public QMainWindow {
        Q_OBJECT

    public:
        MainWindow (QWidget *parent = 0);
        ~MainWindow ();

    protected:
        void changeEvent (QEvent *e);

    private:
        void EnableButtons ();
        void DisableButtons ();

    private:
        Ui::MainWindow *ui;
        DPWorker* mWorker;

private slots:
    void on_simplifiedPolylineCheckBox_toggled(bool checked);
    void on_generatedPolylineCheckBox_toggled(bool checked);
    void on_keepAspectRatioCheckBox_toggled(bool checked);
    void on_simplifyPushButton_clicked ();
    void on_algorithmComboBox_currentIndexChanged(int index);
    void on_generatePushButton_clicked ();
    void SlotGeneratingPolyline ();
    void SlotConvertingPolyline ();
    void SlotSimplifyingPolyline ();
    void SlotCleaningConvertedPolyline ();
    void SlotGeneratedPolyline (int duration, QVector <qreal>& polyline);
    void SlotSimplifiedPolyline (int duration, QVector <qreal>& polyline);
    void SlotSimplifiedPolyline (int duration, QVector <qreal>& polyline, double max, double sum, double mean, double std);
};

} // namespace psimpl


#endif // MAINWINDOW_H
