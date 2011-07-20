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
      unsigned repeat = 25;

// -----------------------------------------------------------------------------

class BenchmarkController
{
public:
    BenchmarkController (unsigned& result) :
        mResult (result),
        mRepeat (repeat)
    {
        mTicks = getticks ();
    }

    ~BenchmarkController () {
        unsigned index = repeat / 2;
        qSort (mMeasurements);
        mResult = mMeasurements [index];
    }

    void StartMeasurement () {
        unsigned measurement = getticks () - mTicks;
        mMeasurements.push_back (measurement);
        --mRepeat;
        mTicks = getticks ();
    }

    bool IsDone () {
        return mRepeat == 0;
    }

private:
    unsigned& mResult;
    unsigned mRepeat;
    CycleCounterTicks mTicks;
   QVector <unsigned> mMeasurements;
};

#define BENCHMARK(elapsed) for (BenchmarkController ctrl (elapsed); !ctrl.IsDone (); ctrl.StartMeasurement ())

// -----------------------------------------------------------------------------

template <class Iterator>
void Benchmark (const std::string& container, double* tmpFirst, double* tmpLast, Iterator first, Iterator last, const Settings& settings)
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

        std::cout << container << "," << setting.first.toStdString () << ",";

        if (setting.first == "simplify_nth_point") {
            BENCHMARK(elapsed) {
                simplificationSize =  std::distance (simplification.get (),
                    psimpl::simplify_nth_point <DIM> (first, last, setting.second [0].toUInt (), simplification.get ()));
            }
            stats = psimpl::compute_positional_error_statistics <DIM> (tmpFirst, tmpLast, simplification.get (), simplification.get () + simplificationSize);
        }
        else if (setting.first == "simplify_radial_distance") {
            BENCHMARK(elapsed) {
                simplificationSize =  std::distance (simplification.get (),
                    psimpl::simplify_radial_distance <DIM> (first, last, setting.second [0].toDouble (), simplification.get ()));
            }
            stats = psimpl::compute_positional_error_statistics <DIM> (tmpFirst, tmpLast, simplification.get (), simplification.get () + simplificationSize);
        }
        else if (setting.first == "simplify_perpendicular_distance") {
            BENCHMARK(elapsed) {
                simplificationSize =  std::distance (simplification.get (),
                    psimpl::simplify_perpendicular_distance <DIM> (first, last, setting.second [0].toDouble (), setting.second [1].toUInt (), simplification.get ()));
            }
            stats = psimpl::compute_positional_error_statistics <DIM> (tmpFirst, tmpLast, simplification.get (), simplification.get () + simplificationSize);
        }
        else if (setting.first == "simplify_reumann_witkam") {
            BENCHMARK(elapsed) {
                simplificationSize =  std::distance (simplification.get (),
                   psimpl::simplify_reumann_witkam <DIM> (first, last, setting.second [0].toDouble (), simplification.get ()));
            }
            stats = psimpl::compute_positional_error_statistics <DIM> (tmpFirst, tmpLast, simplification.get (), simplification.get () + simplificationSize);
        }
        else if (setting.first == "simplify_opheim") {
            BENCHMARK(elapsed) {
                simplificationSize =  std::distance (simplification.get (),
                    psimpl::simplify_opheim <DIM> (first, last, setting.second [0].toDouble (), setting.second [1].toDouble (), simplification.get ()));
            }
            stats = psimpl::compute_positional_error_statistics <DIM> (tmpFirst, tmpLast, simplification.get (), simplification.get () + simplificationSize);
        }
        else if (setting.first == "simplify_lang") {
            BENCHMARK(elapsed) {
                simplificationSize =  std::distance (simplification.get (),
                    psimpl::simplify_lang <DIM> (first, last, setting.second [0].toDouble (), setting.second [1].toUInt (), simplification.get ()));
            }
            stats = psimpl::compute_positional_error_statistics <DIM> (tmpFirst, tmpLast, simplification.get (), simplification.get () + simplificationSize);
        }
        else if (setting.first == "simplify_douglas_peucker") {
            BENCHMARK(elapsed) {
                simplificationSize =  std::distance (simplification.get (),
                    psimpl::simplify_douglas_peucker <DIM> (first, last, setting.second [0].toDouble (), simplification.get ()));
            }
            stats = psimpl::compute_positional_error_statistics <DIM> (tmpFirst, tmpLast, simplification.get (), simplification.get () + simplificationSize);
        }
        else if (setting.first == "simplify_douglas_peucker_n") {
            BENCHMARK(elapsed) {
                simplificationSize =  std::distance (simplification.get (),
                    psimpl::simplify_douglas_peucker_n <DIM> (first, last, setting.second [0].toUInt (), simplification.get ()));
            }
            stats = psimpl::compute_positional_error_statistics <DIM> (tmpFirst, tmpLast, simplification.get (), simplification.get () + simplificationSize);
        }

        std::cout << simplificationSize / DIM << "," << (int) (((float) simplificationSize / (float) polylineSize) * 100.f) << "%," << elapsed << ",";
        std::cout << stats.mean << "," << stats.std << "," << stats.max << ", " << stats.sum << std::endl;
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
    std::cout << "--------------------------------------------------------------------------------" << std::endl;
    std::cout << "cont, algo, size, ratio, ticks, error mean, error std, error max, error sum" << std::endl;
    double* first = polyline.begin ();
    double* last = polyline.end ();
    // test with double[]
    {
        const double* poly = polyline.constData ();
        Benchmark ("double []", first, last, poly, poly + polyline.size (), settings);
    }
    // test with std::vector <double>
    {
        std::vector <double> poly (polyline.constBegin (), polyline.constEnd ());
        Benchmark ("std::vector <double>", first, last, poly.begin (), poly.end (), settings);
    }
    // test with std::deque <double>
    {
        std::deque <double> poly (polyline.constBegin (), polyline.constEnd ());
        Benchmark ("std::deque <double>", first, last, poly.begin (), poly.end (), settings);
    }
    // test with std::list <double>
    {
        std::list <double> poly (polyline.constBegin (), polyline.constEnd ());
        Benchmark ("std::list <double>", first, last, poly.begin (), poly.end (), settings);
    }
}

// -----------------------------------------------------------------------------

int main (int argc, char* argv [])
{
    std::cout << "================================================================================" << std::endl;
    std::cout << "Starting psimpl benchmark" << std::endl;
    if (argc == 2) {
        repeat = QString (argv [1]).toUInt ();
        std::cout << "Setting repeat count to " << repeat << std::endl;
    }

    QDirIterator it (".", QStringList () << "*.algo", QDir::Files | QDir::Readable);
    while (it.hasNext ()) {
        it.next ();
        QFileInfo settings = it.fileInfo ();
        QFileInfo polyline (settings.dir (), settings.completeBaseName ().append (".poly"));
        if (polyline.exists () && polyline.isFile () && polyline.isReadable ()) {
            std::cout << "================================================================================" << std::endl;
            std::cout << "benchmarking : " << settings.completeBaseName ().toStdString ();
            Benchmark (polyline.absoluteFilePath (), settings.absoluteFilePath ());
        }
    }
    std::cout << "================================================================================" << std::endl;

    return 0;
}
