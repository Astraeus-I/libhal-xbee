// Copyright 2024 Khalil Estell
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <libhal-armcortex/dwt_counter.hpp>
#include <libhal-armcortex/startup.hpp>
#include <libhal-armcortex/system_control.hpp>

#include <libhal-lpc40/clock.hpp>
#include <libhal-lpc40/constants.hpp>
#include <libhal-lpc40/i2c.hpp>
#include <libhal-lpc40/uart.hpp>

#include "../hardware_map.hpp"

hal::xbee::hardware_map_t initialize_platform()
{
  using namespace hal::literals;

  // Set the MCU to the maximum clock speed
  hal::lpc40::maximum(12.0_MHz);

  // Create a hardware counter
  static hal::cortex_m::dwt_counter counter(
    hal::lpc40::get_frequency(hal::lpc40::peripheral::cpu));

  static std::array<hal::byte, 512> receive_buffer;
  static std::array<hal::byte, 812> uart1_buffer{};

  // Get and initialize UART0 for UART based logging
  static hal::lpc40::uart uart0(0,
                                receive_buffer,
                                hal::serial::settings{
                                  .baud_rate = 115200,
                                });

  static hal::lpc40::uart uart1(1,
                                uart1_buffer,
                                hal::serial::settings{
                                  .baud_rate = 9600,
                                });

  return {
    .console = &uart0,
    .xbee = &uart1,
    .clock = &counter,
    .reset = []() { hal::cortex_m::reset(); },
  };
}