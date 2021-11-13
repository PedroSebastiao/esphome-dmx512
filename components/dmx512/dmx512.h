#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/output/float_output.h"
#include "esp32-hal-matrix.h"

namespace esphome {
namespace dmx512 {

class DMX512Output;

class DMX512 : public Component {
 public:
  DMX512() = default;
  void set_uart_parent(esphome::uart::UARTComponent *parent) { this->uart_ = parent; }

  void setup();

  void loop() override;

  void dump_config() override;
  
  void sendBreak();

  void set_enable_pin(GPIOPin *pin_enable) { pin_enable_ = pin_enable; }
  
  void set_uart_tx_pin(int tx_pin) { tx_pin_ = tx_pin; }
  
  void set_uart_num(int num) { 
    if(num == 0) {
        this->uart_idx_ = U0TXD_OUT_IDX;
    } else if(num == 1) {
        this->uart_idx_ = U1TXD_OUT_IDX;
    } else if(num == 2) {
        this->uart_idx_ = U2TXD_OUT_IDX;
    }
  }

  float get_setup_priority() const override { return setup_priority::BUS; }

  void write_channel(uint16_t channel, uint8_t value);

 protected:

  esphome::uart::UARTComponent *uart_{nullptr};
  std::vector<uint8_t> rx_buffer_;
  uint32_t last_dmx512_transmission_{0};
  uint8_t device_values[513];
  int uart_idx_{0};
  int tx_pin_{0};
  GPIOPin *pin_enable_{nullptr};
};

class DMX512Output : public output::FloatOutput, public Component {
public:
  void set_universe(DMX512 *universe) { this->universe_ = universe; }
  void set_channel(uint16_t channel) { this->channel_ = channel; }
  void write_state(float state);

protected:
  uint16_t channel_;
  DMX512 *universe_;
};

}  // namespace dmx512
}  // namespace esphome