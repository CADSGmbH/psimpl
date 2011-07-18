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

/*
    psimpl - generic n-dimensional polyline simplification
    Copyright (C) 2010-2011 Elmar de Koning, edekoning@gmail.com

    This file is part of psimpl, and is hosted at SourceForge:
    http://sourceforge.net/projects/psimpl/
*/

#include "../lib/psimpl.h"
#include "../demo/psimpl_reference.h"
#include "cycle_p.h"

#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <QtCore/QDirIterator>
#include <QtCore/QFileInfo>
#include <QtCore/QStringList>
#include <QtCore/QPair>
#include <QtCore/QVector>
#include <QtCore/QTextStream>


typedef QPair <QString, QStringList> Setting;
typedef QVector <Setting> Settings;


const unsigned DIM = 2;
const unsigned repeat = 5;

// -----------------------------------------------------------------------------

class BenchmarkController
{
public:
    BenchmarkController (unsigned& result) : mResult (result), mRepeat (repeat) { mTicks = getticks (); }
    ~BenchmarkController () { mResult = elapsed (getticks (), mTicks) / repeat; }
    void Next () { --mRepeat; }
    bool IsDone () { return mRepeat == 0; }
private:
    unsigned& mResult;
    unsigned mRepeat;
    CycleCounterTicks mTicks;
};

#define BENCHMARK(elapsed) for (BenchmarkController ctrl (elapsed); !ctrl.IsDone (); ctrl.Next ())

// -----------------------------------------------------------------------------

template <class Iterator>
void Benchmark (double* tmpFirst, double* tmpLast, Iterator first, Iterator last, const Settings& settings)
{
    // tmpFirst, tmpLast are the same as first, last; they provide a workaround for a limitation of
    // compute_positional_error_statistics, where all iterators must be of the same type
    int polylineSize = std::distance (first, last);
    unsigned elapsed = 0;

    foreach (const Setting& setting, settings)
    {
        psimpl::util::scoped_array <double> simplification (polylineSize);
        int simplificationSize = 0;
        psimpl::math::Statistics stats;

        std::cout << setting.first.toStdString () << ":\t";

        if (setting.first == "np") {
            BENCHMARK(elapsed) {
                simplificationSize =  std::distance (simplification.get (),
                    psimpl::simplify_nth_point <DIM> (first, last, setting.second [0].toUInt (), simplification.get ()));
            }
            stats = psimpl::compute_positional_error_statistics <DIM> (tmpFirst, tmpLast, simplification.get (), simplification.get () + simplificationSize);
        }
        else if (setting.first == "rd") {
            BENCHMARK(elapsed) {
                simplificationSize =  std::distance (simplification.get (),
                    psimpl::simplify_radial_distance <DIM> (first, last, setting.second [0].toDouble (), simplification.get ()));
            }
            stats = psimpl::compute_positional_error_statistics <DIM> (tmpFirst, tmpLast, simplification.get (), simplification.get () + simplificationSize);
        }
        else if (setting.first == "pd") {
            BENCHMARK(elapsed) {
                simplificationSize =  std::distance (simplification.get (),
                    psimpl::simplify_perpendicular_distance <DIM> (first, last, setting.second [0].toDouble (), setting.second [1].toUInt (), simplification.get ()));
            }
            stats = psimpl::compute_positional_error_statistics <DIM> (tmpFirst, tmpLast, simplification.get (), simplification.get () + simplificationSize);
        }
        else if (setting.first == "rw") {
            BENCHMARK(elapsed) {
                simplificationSize =  std::distance (simplification.get (),
                   psimpl::simplify_reumann_witkam <DIM> (first, last, setting.second [0].toDouble (), simplification.get ()));
            }
            stats = psimpl::compute_positional_error_statistics <DIM> (tmpFirst, tmpLast, simplification.get (), simplification.get () + simplificationSize);
        }
        else if (setting.first == "op") {
            BENCHMARK(elapsed) {
                simplificationSize =  std::distance (simplification.get (),
                    psimpl::simplify_opheim <DIM> (first, last, setting.second [0].toDouble (), setting.second [1].toDouble (), simplification.get ()));
            }
            stats = psimpl::compute_positional_error_statistics <DIM> (tmpFirst, tmpLast, simplification.get (), simplification.get () + simplificationSize);
        }
        else if (setting.first == "la") {
            BENCHMARK(elapsed) {
                simplificationSize =  std::distance (simplification.get (),
                    psimpl::simplify_lang <DIM> (first, last, setting.second [0].toDouble (), setting.second [1].toUInt (), simplification.get ()));
            }
            stats = psimpl::compute_positional_error_statistics <DIM> (tmpFirst, tmpLast, simplification.get (), simplification.get () + simplificationSize);
        }
        else if (setting.first == "dp") {
            BENCHMARK(elapsed) {
                simplificationSize =  std::distance (simplification.get (),
                    psimpl::simplify_douglas_peucker <DIM> (first, last, setting.second [0].toDouble (), simplification.get ()));
            }
            stats = psimpl::compute_positional_error_statistics <DIM> (tmpFirst, tmpLast, simplification.get (), simplification.get () + simplificationSize);
        }
        else if (setting.first == "dpn") {
            BENCHMARK(elapsed) {
                simplificationSize =  std::distance (simplification.get (),
                    psimpl::simplify_douglas_peucker_n <DIM> (first, last, setting.second [0].toUInt (), simplification.get ()));
            }
            stats = psimpl::compute_positional_error_statistics <DIM> (tmpFirst, tmpLast, simplification.get (), simplification.get () + simplificationSize);
        }
        else if (setting.first == "dpr") {
            std::cout << std::endl;
            continue;
        }

        std::cout << simplificationSize / DIM << " (" << ((float) simplificationSize / (float) polylineSize) * 100.f << "%)\t" << elapsed << "\t";
        std::cout << stats.mean << ", " << stats.std << ", " << stats.max << ", " << stats.sum << std::endl;
    }
}

// -----------------------------------------------------------------------------

void Benchmark (const QString& polylinePath, const QString& settingsPath)
{
    QVector <double> polyline;
    Settings settings;

    // load polyline
    {
        QFile polylineFile (polylinePath);
        if (!polylineFile.open (QIODevice::ReadOnly | QIODevice::Text)) {
            std::cout << "unable to read " << polylinePath.toStdString () << std::endl;
            return;
        }
        QTextStream in (&polylineFile);
        while (!in.atEnd ()) {
            QString line = in.readLine ();
            QStringList list = line.split (',', QString::SkipEmptyParts);
            if (list.size () < 2) {
                std::cout << "invalid line read from " << polylinePath.toStdString () << std::endl;
                return;
            }
            polyline.push_back (list [0].toDouble ());
            polyline.push_back (list [1].toDouble ());
        }
        std::cout << ", " << polyline.size () / DIM << " points";
    }
    // load settings
    {
        QFile settingsFile (settingsPath);
        if (!settingsFile.open (QIODevice::ReadOnly | QIODevice::Text)) {
            std::cout << "unable to read " << settingsPath.toStdString () << std::endl;
            return;
        }
        QTextStream in (&settingsFile);
        while (!in.atEnd ()) {
            QString line = in.readLine ();
            QString algo = line.section (',', 0, 0);
            QStringList params = line.section (',', 1).split (',', QString::SkipEmptyParts);
            if (0 == algo.size () || 0 == params.size ()) {
                std::cout << "invalid line read from " << settingsPath.toStdString () << std::endl;
                return;
            }
            settings.push_back (qMakePair (algo, params));
        }
        std::cout << ", " << settings.size () << " algorithms" << std::endl;
    }
    double* first = polyline.begin ();
    double* last = polyline.end ();
    // test with double[]
    {
        std::cout << "--------------------------------------------------------------------------------" << std::endl;
        std::cout << "using: double*" << std::endl;
        const double* poly = polyline.constData ();
        Benchmark (first, last, poly, poly + polyline.size (), settings);
    }
    // test with std::vector <double>
    {
        std::cout << "--------------------------------------------------------------------------------" << std::endl;
        std::cout << "using: std::vector <double>" << std::endl;
        std::vector <double> poly (polyline.constBegin (), polyline.constEnd ());
        Benchmark (first, last, poly.begin (), poly.end (), settings);
    }
    // test with std::deque <double>
    {
        std::cout << "--------------------------------------------------------------------------------" << std::endl;
        std::cout << "using: std::deque <double>" << std::endl;
        std::deque <double> poly (polyline.constBegin (), polyline.constEnd ());
        Benchmark (first, last, poly.begin (), poly.end (), settings);
    }
    // test with std::list <double>
    {
        std::cout << "--------------------------------------------------------------------------------" << std::endl;
        std::cout << "using: std::list <double>" << std::endl;
        std::list <double> poly (polyline.constBegin (), polyline.constEnd ());
        Benchmark (first, last, poly.begin (), poly.end (), settings);
    }
}

// -----------------------------------------------------------------------------

int main (int /*argc*/, char * /*argv*/ [])
{
    QDirIterator it (".", QStringList () << "*.algo", QDir::Files | QDir::Readable);
    while (it.hasNext ()) {
        it.next ();
        QFileInfo settings = it.fileInfo ();
        QFileInfo polyline (settings.dir (), settings.completeBaseName ().append (".poly"));
        if (polyline.exists () && polyline.isFile () && polyline.isReadable ()) {
            std::cout << "================================================================================" << std::endl;
            std::cout << "benchmarking : " << settings.completeBaseName ().toStdString ();
            Benchmark (polyline.absoluteFilePath (), settings.absoluteFilePath ());
            std::cout << "================================================================================" << std::endl;
        }
    }

    return 0;
}
