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


#include <string>
#include <rtxi/widgets.hpp>

// This is an generated header file. You may change the namespace, but 
// make sure to do the same in implementation (.cpp) file
namespace ttl_pulses
{

constexpr std::string_view MODULE_NAME = "ttl-pulses";

enum PARAMETER : Widgets::Variable::Id
{
    // set parameter ids here
    TTL_DURATION=0,
    TTL_PULSES,
    TTL_FREQUENCY,
    TTL_DELAY,
    REPEAT,
    CURRENT_TRIAL,
    CURRENT_PULSE
};

inline  std::vector<Widgets::Variable::Info> get_default_vars() 
{
  return {
    {
      TTL_DURATION,
      "TTL Duration (s)", 
      "Duration of pulse", 
      Widgets::Variable::DOUBLE_PARAMETER,
      0.25
    },
    {
      TTL_PULSES,
      "TTL Pulses (#)", 
      "Number of pulses", 
      Widgets::Variable::UINT_PARAMETER,
      uint64_t{1}
    },
    {
      TTL_FREQUENCY,
      "TTL Freq (Hz)", 
      "Frequency measured between pulse onsets",
      Widgets::Variable::DOUBLE_PARAMETER,
      1.0
    },
    {
      TTL_DELAY,
      "TTL Delay (s)", 
      "Time to wait between trials",
      Widgets::Variable::DOUBLE_PARAMETER,
      0.5
    },
    {
      REPEAT,
      "Repeat (#)", 
      "Number of trials", 
      Widgets::Variable::UINT_PARAMETER,
      uint64_t{1}
    },
    {
      CURRENT_TRIAL,
      "Current Trial", 
      "Current trial being executed", 
      Widgets::Variable::STATE,
      uint64_t{1}
    },
    {
      CURRENT_PULSE,
      "Current Pulse", 
      "Current pulse being executed", 
      Widgets::Variable::STATE,
      uint64_t{1}
    }
  };
}

inline std::vector<IO::channel_t> get_default_channels()
{
  return {
      { "TTL Signal", "TTL Signal", IO::OUTPUT, }, // output(0)
  };
}

class Panel : public Widgets::Panel
{
  Q_OBJECT
public:
  Panel(QMainWindow* main_window, Event::Manager* ev_manager);
};

class Component : public Widgets::Component
{
public:
  explicit Component(Widgets::Plugin* hplugin);
  void execute() override;

private:
  int64_t t_zero;
};

class Plugin : public Widgets::Plugin
{
public:
  explicit Plugin(Event::Manager* ev_manager);
};

}  // namespace ttl_pulses
