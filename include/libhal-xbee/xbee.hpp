// Copyright 2023 Google LLC
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

#pragma once

#include <array>
#include <cstdint>
#include <string_view>

#include <libhal-util/steady_clock.hpp>
#include <libhal/functional.hpp>
#include <libhal/serial.hpp>
#include <libhal/steady_clock.hpp>
#

namespace hal::xbee {
class xbee_radio
{
public:

  explicit xbee_radio(hal::serial& p_serial, hal::steady_clock& p_clock);

  std::span<hal::byte> read();

  void write(std::span<const hal::byte> p_data);

  void configure_xbee(const char* p_channel, const char* p_panid);

private:

  hal::serial* m_serial;
  hal::steady_clock* m_clock;

  std::array<hal::byte, 256> m_xbee_buffer;

  void write(const char* str);
  void write_command(const char* command, const char* value);
};

}  // namespace hal::xbee