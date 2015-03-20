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

#include <default_gui_model.h>
#include <string>

class TTL : public DefaultGUIModel
{

public:
    TTL(void);
    virtual ~TTL(void);
    virtual void execute(void);

protected:
    virtual void update(DefaultGUIModel::update_flags_t);

private:
    double dt;
    double ttlDuration; // s
    double ttlNumPulses;
    double ttlFreq; // Hz
    double ttlDelay; // s
    double maxtrials;
    int trial;
    int idx;
    std::vector<double> stimTTL;

private slots:
    void initParameters();
    void bookkeep();
    void makeTTL();
};
