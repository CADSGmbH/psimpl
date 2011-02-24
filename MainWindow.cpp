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
 * The Original Code is 'psimpl - generic n-dimensional polyline simplification'.
 *
 * The Initial Developer of the Original Code is
 * Elmar de Koning.
 * Portions created by the Initial Developer are Copyright (C) 2010
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * ***** END LICENSE BLOCK ***** */
 
/*!
    psimpl - generic n-dimensional polyline simplification
    Copyright (C) 2010 Elmar de Koning, edekoning@gmail.com

    This file is part of psimpl and was originally released as part
    of the article 'Polyline Simplification' at The Code Project:
    http://www.codeproject.com/KB/recipes/PolylineSimplification.aspx

    28-09-2010 - Initial version
    23-10-2010 - Changed license from CPOL to MPL
    26-10-2010 - Clarified input (type) requirements, and changed the
                 behavior of the algorithms under invalid input
    01-12-2010 - Added the nth point, perpendicular distance and Reumann-Witkam routines;
                 moved all functions related to distance calculations to the math namespace
    10-12-2010 - Fixed a bug in the perpendicular distance routine
*/

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "DPWorker.h"
#include <cmath>
#include <QtGui/QToolButton>


namespace psimpl {

    MainWindow::MainWindow (QWidget *parent) :
        QMainWindow (parent),
        ui (new Ui::MainWindow)
    {
        mWorker = new DPWorker (this);

        ui->setupUi (this);

        QToolButton* about = new QToolButton(this);
        about->setDefaultAction (ui->aboutAction);
        ui->statusBar->addPermanentWidget(about);

        // Lang not yet implemented
        int LANG = 5;
        ui->algorithmComboBox->removeItem (LANG);
        ui->labelStackedWidget->removeWidget (ui->labelStackedWidget->widget (LANG));
        ui->editStackedWidget->removeWidget (ui->editStackedWidget->widget (LANG));

#ifndef _DEBUG
        ui->algorithmComboBox->removeItem (DOUGLAS_PEUCKER_REFERENCE);
#endif
        connect (mWorker, SIGNAL (SignalGeneratingPolyline ()),
                 this, SLOT (SlotGeneratingPolyline ()));
        connect (mWorker, SIGNAL (SignalConvertingPolyline ()),
                 this, SLOT (SlotConvertingPolyline ()));
        connect (mWorker, SIGNAL (SignalSimplifyingPolyline ()),
                 this, SLOT (SlotSimplifyingPolyline ()));
        connect (mWorker, SIGNAL (SignalCleaningConvertedPolyline ()),
                 this, SLOT (SlotCleaningConvertedPolyline ()));

        connect (mWorker, SIGNAL (SignalGeneratedPolyline (int, QVector <qreal>&)),
                 this, SLOT (SlotGeneratedPolyline (int, QVector <qreal>&)));
        connect (mWorker, SIGNAL (SignalSimplifiedPolyline (int, QVector <qreal>&)),
                 this, SLOT (SlotSimplifiedPolyline (int, QVector <qreal>&)));
        connect (mWorker, SIGNAL (SignalSimplifiedPolyline (int, QVector <qreal>&, double, double, double, double)),
                 this, SLOT (SlotSimplifiedPolyline (int, QVector <qreal>&, double, double, double, double)));
    }

    MainWindow::~MainWindow ()
    {
        delete ui;
        delete mWorker;
    }

    void MainWindow::changeEvent (QEvent *e)
    {
        QMainWindow::changeEvent (e);

        switch (e->type ()) {
        case QEvent::LanguageChange:
            ui->retranslateUi (this);
            break;

        default:
            break;
        }
    }

    void MainWindow::EnableButtons ()
    {
        ui->generatePushButton->setEnabled (true);
        ui->simplifyPushButton->setEnabled (true);
        ui->togglePushButton->setEnabled (true);
    }

    void MainWindow::DisableButtons ()
    {
        ui->generatePushButton->setDisabled (true);
        ui->simplifyPushButton->setDisabled (true);
        ui->togglePushButton->setDisabled (true);
    }

    void MainWindow::on_generatePushButton_clicked ()
    {
        QApplication::setOverrideCursor (QCursor (Qt::WaitCursor));
        DisableButtons ();
        mWorker->Generate (ui->polyPointCountSpinBox->value ());
    }

    void MainWindow::on_simplifyPushButton_clicked ()
    {
        QApplication::setOverrideCursor (QCursor (Qt::WaitCursor));
        DisableButtons ();

        switch (ui->algorithmComboBox->currentIndex ())
        {
        case NTH_POINT:
            mWorker->SimplifyNP ((Container)ui->polyTypeComboBox->currentIndex (), ui->npSpinBox->value ());
            break;

        case RADIAL_DISTANCE:
            mWorker->SimplifyRD ((Container)ui->polyTypeComboBox->currentIndex (), ui->rdLineEdit->text ());
            break;

        case PERPENDICULAR_DISTANCE:
            mWorker->SimplifyPD ((Container)ui->polyTypeComboBox->currentIndex (), ui->pdLineEdit->text (), ui->pdSpinBox->value ());
            break;

        case REUMANN_WITKAM:
            mWorker->SimplifyRW ((Container)ui->polyTypeComboBox->currentIndex (), ui->rwLineEdit->text ());
            break;

        case OPHEIM:
            mWorker->SimplifyOp ((Container)ui->polyTypeComboBox->currentIndex (), ui->minOpLineEdit->text (), ui->maxOpLineEdit->text ());
            break;

        //case LANG:
        //    mWorker->SimplifyLa ((Container)ui->polyTypeComboBox->currentIndex (), ui->laLineEdit->text (), ui->sizeLaSpinBox->value ());
        //    break;

        case DOUGLAS_PEUCKER:
            mWorker->SimplifyDP ((Container)ui->polyTypeComboBox->currentIndex (), ui->dpLineEdit->text ());
            break;

        case DOUGLAS_PEUCKER_VARIANT:
            mWorker->SimplifyDP_variant ((Container)ui->polyTypeComboBox->currentIndex (), ui->dpvSpinBox->value ());
            break;

        case DOUGLAS_PEUCKER_REFERENCE:
            mWorker->SimplifyDP_reference (ui->dprLineEdit->text ());
            break;
        }
    }

    void MainWindow::on_algorithmComboBox_currentIndexChanged (int index)
    {
        if (index == DOUGLAS_PEUCKER_REFERENCE) {
            ui->polyTypeComboBox->setCurrentIndex (ARRAY_FLOAT);
            ui->polyTypeComboBox->setDisabled (true);
        }
        else {
            ui->polyTypeComboBox->setEnabled (true);
        }
    }

    void MainWindow::on_generatedPolylineCheckBox_toggled (bool checked)
    {
        ui->renderArea->SetVisibleGeneratedPolyline (checked);
        ui->togglePushButton->setDisabled(
            ui->generatedPolylineCheckBox->isChecked () ==
            ui->simplifiedPolylineCheckBox->isChecked ());
        update();
    }

    void MainWindow::on_simplifiedPolylineCheckBox_toggled (bool checked)
    {
        ui->renderArea->SetVisibleSimplifiedPolyline (checked);
        ui->togglePushButton->setDisabled(
            ui->generatedPolylineCheckBox->isChecked () ==
            ui->simplifiedPolylineCheckBox->isChecked ());
        update();
    }

    void MainWindow::on_keepAspectRatioCheckBox_toggled (bool checked)
    {
        ui->renderArea->SetKeepAspectRatio (checked);
        update();
    }

    void MainWindow::SlotGeneratingPolyline () {
        ui->statusBar->showMessage ("Generating polyline...");
    }

    void MainWindow::SlotConvertingPolyline () {
        ui->statusBar->showMessage (QString ("Converting polyline to '%1'...").arg (ui->polyTypeComboBox->currentText ()));
    }

    void MainWindow::SlotSimplifyingPolyline () {
        ui->statusBar->showMessage ("Simplifying polyline...");
    }

    void MainWindow::SlotCleaningConvertedPolyline () {
        ui->statusBar->showMessage ("Deleting converted polyline...");
    }

    void MainWindow::SlotGeneratedPolyline (int duration, QVector <qreal>& polyline)
    {
        ui->statusBar->showMessage (QString ("Generation took %1 ms").arg (duration));
        ui->renderArea->SetGeneratedPolyline (polyline);
        ui->simplGroupBox->setEnabled (true);
        EnableButtons ();
        QApplication::restoreOverrideCursor ();
        update();
    }

    void MainWindow::SlotSimplifiedPolyline (int duration, QVector <qreal>& polyline)
    {
        int pointCount = polyline.count () / 2;
        ui->maxValueLabel->setText ("-");
        ui->sumValueLabel->setText ("-");
        ui->meanValueLabel->setText ("-");
        ui->stdValueLabel->setText ("-");
        ui->statusBar->showMessage (
            QString ("Simplification took %1 ms; %2 (%3%) points remaining").
                arg (duration).
                arg (pointCount).
                arg (100.0 * pointCount / mWorker->GetGeneratedPointCount()));
        ui->renderArea->SetSimplifiedPolyline(polyline);
        EnableButtons ();
        QApplication::restoreOverrideCursor ();
        update();
    }

    void MainWindow::SlotSimplifiedPolyline (int duration, QVector <qreal>& polyline, double max, double sum, double mean, double std)
    {
        int pointCount = polyline.count () / 2;
        ui->maxValueLabel->setText (QString::number (max));
        ui->sumValueLabel->setText (QString::number (sum));
        ui->meanValueLabel->setText (QString::number (mean));
        ui->stdValueLabel->setText (QString::number (std));
        ui->statusBar->showMessage (
            QString ("Simplification took %1 ms; %2 (%3%) points remaining").
                arg (duration).
                arg (pointCount).
                arg (100.0 * pointCount / mWorker->GetGeneratedPointCount()));
        ui->renderArea->SetSimplifiedPolyline(polyline);
        EnableButtons ();
        QApplication::restoreOverrideCursor ();
        update();
    }
} // namespace psimpl
