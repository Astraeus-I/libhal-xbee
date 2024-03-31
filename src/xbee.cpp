#include "libhal-xbee/xbee.hpp"

#include <algorithm>
#include <array>
#include <span>
#include <cstring>

#include <libhal-util/serial.hpp>
#include <libhal-util/streams.hpp>
#include <libhal/serial.hpp>

namespace hal::xbee {

xbee_radio::xbee_radio(hal::serial& p_serial, hal::steady_clock& p_clock)
  : m_serial(&p_serial)
  , m_clock(&p_clock)
{
}

std::span<hal::byte> xbee_radio::read()
{
  auto data_recieved = m_serial->read(m_xbee_buffer).data;
  return data_recieved;
}

void xbee_radio::write(std::span<const hal::byte> p_data)
{
  m_serial->write(p_data);
}

void xbee_radio::write(const char* str)
{
  auto length = strlen(str);
  std::span<const hal::byte> span(reinterpret_cast<const hal::byte*>(str),
                                  length);
  write(span);
}

void xbee_radio::configure_xbee(const char* p_channel,
                                       const char* p_panid)
{

  using namespace std::chrono_literals;
  using namespace hal::literals;

  hal::delay(*m_clock, 500ms);
  write("+++");
  hal::delay(*m_clock, 100ms);
  auto output = m_serial->read(m_xbee_buffer).data;
  hal::delay(*m_clock, 1000ms);

  int retry_count = 0;
  const int max_retries = 5;  // Define a maximum number of retries

  while (retry_count < max_retries) {
    // Try to enter command mode
    write("+++");
    hal::delay(*m_clock, 100ms);
    output = m_serial->read(m_xbee_buffer).data;
    hal::delay(*m_clock, 1000ms);

    if (output[0] == 'O' && output[1] == 'K') {
      hal::print(*m_serial, "Radio Success\r");
      break;
    } else if (output[0] == 'E' && output[1] == 'R' && output[2] == 'R') {
      hal::print(*m_serial, "Error Occurred\r");
      retry_count++;
      hal::delay(*m_clock, 2000ms);  // Optional delay before retrying
    } else {
      hal::print(*m_serial, "Unknown response\r");
      retry_count++;
      hal::delay(*m_clock, 2000ms);  // Optional delay before retrying
    }
  }

  if (retry_count == max_retries) {
    hal::print(*m_serial,
               "Failed to enter command mode after multiple attempts\r");
    // Handle the failure here, perhaps by returning an error or taking other
    // actions
  }

  // // Set channel
  write_command("ATCH", p_channel);
  hal::delay(*m_clock, 100ms);
  // Set PAN ID
  write_command("ATID", p_panid);
  hal::delay(*m_clock, 100ms);
  // // Set Baud Rate to 115200
  write("ATBD7");
  hal::delay(*m_clock, 100ms);
  // Save configuration
  write("ATWR\r");
  hal::delay(*m_clock, 100ms);
  // Exit command mode
  write("ATCN\r");
  hal::delay(*m_clock, 100ms);
}

void xbee_radio::write_command(const char* command, const char* value)
{
  write(command);
  write(value);
  write("\r");
}

}  // namespace hal::xbee