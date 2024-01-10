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

#include <QTimer>

#include "widget.hpp"

ttl_pulses::Plugin::Plugin(Event::Manager* ev_manager)
    : Widgets::Plugin(ev_manager, std::string(ttl_pulses::MODULE_NAME))
{
}

ttl_pulses::Panel::Panel(QMainWindow* main_window, Event::Manager* ev_manager)
    : Widgets::Panel(
        std::string(ttl_pulses::MODULE_NAME), main_window, ev_manager)
{
  setWhatsThis(
      "<p><b>TTL:</b><br>This module outputs a train of TTL pulses (0-5V "
      "square wave). This can"
      "be connected to either an analog or digital output channel.</p>");
  createGUI(ttl_pulses::get_default_vars(),
            {});  // this is required to create the GUI
  refresh();
  QTimer::singleShot(0, this, SLOT(resizeMe()));
}

ttl_pulses::Component::Component(Widgets::Plugin* hplugin)
    : Widgets::Component(hplugin,
                         std::string(ttl_pulses::MODULE_NAME),
                         ttl_pulses::get_default_channels(),
                         ttl_pulses::get_default_vars())
{
}

void ttl_pulses::Component::execute()
{
  // ttl-pulses used to allocate a vector every time the user
  // modified constants. This would cause strain on the real-time
  // thread. This new execute function avoids allocating a vector,
  // and simply computes what the voltage should be during
  // runtime.
  constexpr std::array<double, 2> voltages = {0.0, 5.0};
  const int64_t time = RT::OS::getTime();
  const auto dt = static_cast<uint64_t>(time - t_zero);
  const auto ttldelay_ns = static_cast<uint64_t>(
      getValue<double>(TTL_DELAY) * RT::OS::SECONDS_TO_NANOSECONDS);
  const auto ttlduration_ns = static_cast<uint64_t>(
      getValue<double>(TTL_DURATION) * RT::OS::SECONDS_TO_NANOSECONDS);
  const auto ttlperiod_ns = static_cast<uint64_t>(RT::OS::SECONDS_TO_NANOSECONDS/getValue<double>(TTL_FREQUENCY));
  auto ttl_current_pulse = getValue<uint64_t>(CURRENT_PULSE);
  const bool on = dt > ttldelay_ns && (dt - ttldelay_ns) <= (ttlduration_ns + (ttlperiod_ns * (ttl_current_pulse-1)));
  switch (this->getState()) {
    case RT::State::EXEC:
      writeoutput(0, voltages[static_cast<size_t>(on)]);
      if(dt > (ttlperiod_ns*ttl_current_pulse + ttldelay_ns)) {
        setValue<uint64_t>(CURRENT_PULSE, ttl_current_pulse+1);
        ttl_current_pulse++;
      }
      if(ttl_current_pulse > getValue<uint64_t>(TTL_PULSES)){
        t_zero = time;
        setValue<uint64_t>(CURRENT_TRIAL, getValue<uint64_t>(CURRENT_TRIAL) + 1);
        setValue<uint64_t>(CURRENT_PULSE, 1);
      }
      if(getValue<uint64_t>(CURRENT_TRIAL) > getValue<uint64_t>(REPEAT)){
        setState(RT::State::PAUSE);
      }
      break;
    case RT::State::INIT:
    case RT::State::MODIFY:
      writeoutput(0, 0);
      setState(RT::State::PAUSE);
      break;
    case RT::State::PERIOD:
      writeoutput(0, 0);
      setState(RT::State::MODIFY);
      break;
    case RT::State::PAUSE:
      writeoutput(0, 0);
      break;
    case RT::State::UNPAUSE:
      writeoutput(0, 0);
      t_zero = time;
      setValue<uint64_t>(CURRENT_PULSE, 1);
      setValue<uint64_t>(CURRENT_TRIAL, 1);
      setState(RT::State::EXEC);
      break;
    default:
      break;
  }
}

///////// DO NOT MODIFY BELOW //////////
// The exception is if your plugin is not going to need real-time functionality.
// For this case just replace the craeteRTXIComponent return type to nullptr.
// RTXI will automatically handle that case and won't attach a component to the
// real time thread for your plugin.

std::unique_ptr<Widgets::Plugin> createRTXIPlugin(Event::Manager* ev_manager)
{
  return std::make_unique<ttl_pulses::Plugin>(ev_manager);
}

Widgets::Panel* createRTXIPanel(QMainWindow* main_window,
                                Event::Manager* ev_manager)
{
  return new ttl_pulses::Panel(main_window, ev_manager);
}

std::unique_ptr<Widgets::Component> createRTXIComponent(
    Widgets::Plugin* host_plugin)
{
  return std::make_unique<ttl_pulses::Component>(host_plugin);
}

Widgets::FactoryMethods fact;

extern "C"
{
Widgets::FactoryMethods* getFactories()
{
  fact.createPanel = &createRTXIPanel;
  fact.createComponent = &createRTXIComponent;
  fact.createPlugin = &createRTXIPlugin;
  return &fact;
}
}

//////////// END //////////////////////
