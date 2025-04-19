#pragma once
#include "udscom/can_backend.hpp"
#include <vector>

/// A trivially deterministic replacement for the real SocketCAN / PCAN backend
class MockBackend : public uds::CanBackend {
public:
    std::vector<std::uint8_t> canned_resp;          // set this in each test

    void open(const std::string&, uint32_t, uint32_t) override { /* noop */ }

    std::vector<std::uint8_t>
    request(std::span<const std::uint8_t>,     // the outgoing frame
            std::chrono::milliseconds) override
    {
        return canned_resp;                    // always hand back the preset data
    }
};