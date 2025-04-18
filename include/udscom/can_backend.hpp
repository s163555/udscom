#pragma once
#include <cstdint>
#include <vector>
#include <memory>
#include <span>
#include <chrono>

class CanBackend {
public:
    virtual ~CanBackend()                                    = default;
    virtual void open(std::string_view iface,
                      uint32_t rx_id, uint32_t tx_id)       = 0;
    virtual std::vector<uint8_t> request(std::span<const uint8_t> bytes,
                                         std::chrono::milliseconds to) = 0;
};
std::unique_ptr<CanBackend> make_backend();
