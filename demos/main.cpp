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

#include <libhal/error.hpp>
#include "hardware_map.hpp"

hal::xbee::hardware_map_t hardware_map{};
volatile std::errc init_error_code{};

int main()
{
  try {
    hardware_map = initialize_platform();
  } catch (const hal::exception& p_error) {
    init_error_code = p_error.error_code();
    hal::halt();
  }

  application(hardware_map);

  return 0;
}