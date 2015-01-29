/*
Copyright (C) 2011 Georgia Institute of Technology

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <ttl.h>
#include <QtGui>

extern "C" Plugin::Object *createRTXIPlugin(void)
{
    return new TTL();
}

static DefaultGUIModel::variable_t vars[] = {
    { "TTL Signal", "TTL Signal", DefaultGUIModel::OUTPUT, }, // output(0)
    {
        "TTL Duration (s)", "Duration of pulse", DefaultGUIModel::PARAMETER
        | DefaultGUIModel::DOUBLE,
    },
    {
        "TTL Pulses (#)", "Number of pulses", DefaultGUIModel::PARAMETER
        | DefaultGUIModel::DOUBLE,
    },
    {
        "TTL Freq (Hz)", "Frequency measured between pulse onsets",
        DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE,
    },
    {
        "TTL Delay (s)", "Time to wait between trials",
        DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE,
    },
    {
        "Repeat", "Number of trials", DefaultGUIModel::PARAMETER
        | DefaultGUIModel::DOUBLE,
    },
};

static size_t num_vars = sizeof(vars) / sizeof(DefaultGUIModel::variable_t);

TTL::TTL(void) : DefaultGUIModel("TTL", ::vars, ::num_vars), dt(RT::System::getInstance()->getPeriod() * 1e-9), ttlDuration(.25), ttlNumPulses(1), ttlFreq(1), ttlDelay(.5), maxtrials(1), trial(0), idx(0)
{
    setWhatsThis(
        "<p><b>TTL:</b><br>This module outputs a train of TTL pulses (0-5V square wave). This can"
        "be connected to either an analog or digital output channel.</p>");
    DefaultGUIModel::createGUI(vars, num_vars);
    initParameters();
    update( INIT);
    refresh();
    QTimer::singleShot(0, this, SLOT(resizeMe()));
}

TTL::~TTL(void) {}

void TTL::execute(void)
{
    if (trial < maxtrials) { // run trial
        if (idx < stimTTL.size()) {
            output(0) = stimTTL[idx++];
        } else {
            trial++;
            idx = 0;
        }
    } // end single trial
    else { // all trials are done, send signal to holding current module, and pause
        output(0) = 0;
        pause(true);
    } // end protocol
}

void TTL::update(DefaultGUIModel::update_flags_t flag)
{
    switch (flag) {
    case INIT:
        setParameter("Repeat", QString::number(maxtrials)); // initially 1
        setParameter("TTL Duration (s)", QString::number(ttlDuration)); // initially 1
        setParameter("TTL Pulses (#)", QString::number(ttlNumPulses)); // initially 1
        setParameter("TTL Freq (Hz)", QString::number(ttlFreq)); // initially 1
        setParameter("TTL Delay (s)", QString::number(ttlDelay)); // initially 1
        break;

    case MODIFY:
        if (getParameter("TTL Duration (s)").toDouble() >= (getParameter("TTL Freq (Hz)").toDouble())) {
            QMessageBox::critical(this,"TTL",tr(
                                      "The TTL pulses will overlap with the specified pulse duration and frequency.\n"));
        } else {
            ttlDuration = getParameter("TTL Duration (s)").toDouble();
            ttlFreq = getParameter("TTL Freq (Hz)").toDouble();
            ttlNumPulses = getParameter("TTL Pulses (#)").toDouble();
            ttlDelay = getParameter("TTL Delay (s)").toDouble();
            makeTTL();
        }
        maxtrials = getParameter("Repeat").toDouble();
        makeTTL();
        break;

    case PAUSE:
        output(0) = 0;
        bookkeep();
        break;

    case PERIOD:
        dt = RT::System::getInstance()->getPeriod() * 1e-9; // sec
        idx = 0;
        break;

    default:
        break;
    }
}

// custom functions
void TTL::initParameters()
{
    maxtrials = 1;
    ttlDuration = .25; // s
    ttlNumPulses = 1;
    ttlFreq = 1; // Hz
    ttlDelay = .5; // s
    bookkeep();
    makeTTL();
}

void TTL::bookkeep()
{
    trial = 0;
    idx = 0;
}

void TTL::makeTTL()
{
    stimTTL.clear();
    for (int i = 0; i < ttlDelay / dt; i++) { // initial delay in trial before starting ttl
        stimTTL.push_back(0);
    }
    for (int n = 0; n < ttlNumPulses; n++) {
        for (int i = 0; i < ttlDuration / dt; i++) {
            stimTTL.push_back(5);
        }
        // fill in zeros for frequency
        for (int i = 0; i < ((1 / ttlFreq) - ttlDuration) / dt; i++) {
            stimTTL.push_back(0);
        }
    }
}
