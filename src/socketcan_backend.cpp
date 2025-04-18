#include "udscom/can_backend.hpp"

#include <array>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/isotp.h>
#include <net/if.h>
#include <unistd.h>         // close()
#include <poll.h>
#include <errno.h>

namespace {

static uint32_t make_can_id(uint32_t id) {
    return (id > 0x7FF) ? (id | CAN_EFF_FLAG) : id;
}

/* helper ---------------------------------------------------------------- */
[[noreturn]] void throw_errno(const char* msg) {
    throw std::runtime_error(std::string(msg) + ": " + std::strerror(errno));
}

/* convert "can0"  →  ifindex ------------------------------------------- */
int ifindex_from_name(const std::string& name) {
    struct ifreq ifr {};
    std::strncpy(ifr.ifr_name, name.c_str(), IFNAMSIZ - 1);
    int s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (s < 0) throw_errno("socket(PF_CAN)");
    if (ioctl(s, SIOCGIFINDEX, &ifr) < 0) {
        ::close(s);
        throw_errno("ioctl(SIOCGIFINDEX)");
    }
    ::close(s);
    return ifr.ifr_ifindex;
}

} // unnamed namespace

/* ====================================================================== *
   concrete implementation of the abstract CanBackend interface
 * ====================================================================== */
class SocketCanBackend final : public CanBackend {
public:
    SocketCanBackend()  = default;
    ~SocketCanBackend() override { close_socket(); }

    /* -------------------------------------------------- open ---------- */
    void open(std::string_view iface,
              uint32_t rx_id,
              uint32_t tx_id) override
    {
        close_socket();                       // in case we are re‑opened

        sock_ = ::socket(PF_CAN, SOCK_DGRAM, CAN_ISOTP);
        if (sock_ < 0) throw_errno("socket(CAN_ISOTP)");

        /* optional:  set a small default rx timeout so that read()
                      returns EAGAIN instead of blocking forever          */
        struct timeval tv {1, 0};             // 1 s
        ::setsockopt(sock_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof tv);

        struct sockaddr_can addr {};
        addr.can_family          = AF_CAN;
        addr.can_ifindex         = ifindex_from_name(std::string(iface));
        addr.can_addr.tp.rx_id   = make_can_id(rx_id);   // <‑‑ add flag if needed
        addr.can_addr.tp.tx_id   = make_can_id(tx_id);   // <‑‑ add flag if needed

        if (::bind(sock_, reinterpret_cast<sockaddr*>(&addr), sizeof addr) < 0)
            throw_errno("bind(iso‑tp)");

        iface_name_ = iface;
    }

    /* ------------------------------------------------ request --------- */
    std::vector<std::uint8_t>
    request(std::span<const std::uint8_t> bytes,
            std::chrono::milliseconds timeout) override
    {
        if (sock_ < 0)
            throw std::logic_error("SocketCanBackend not opened");

        /* ------------------------------------------------------------------
           1) send request                                                   */
        ssize_t sent = ::write(sock_, bytes.data(), bytes.size());
        if (sent < 0) throw_errno("write(iso‑tp)");

        /* ------------------------------------------------------------------
           2) wait for response or timeout using poll()                      */
        struct pollfd pfd { sock_, POLLIN, 0 };
        int rv = ::poll(&pfd, 1, static_cast<int>(timeout.count()));
        if (rv < 0)  throw_errno("poll()");
        if (rv == 0) return {};                // → timeout → empty vector

        /* ------------------------------------------------------------------
           3) read full, kernel‑reassembled ISO‑TP frame                     */
        std::array<std::uint8_t, 4096> buf {};
        ssize_t n = ::read(sock_, buf.data(), buf.size());
        if (n < 0)  throw_errno("read(iso‑tp)");

        return {buf.begin(), buf.begin() + n};
    }

private:
    int         sock_       = -1;
    std::string iface_name_;

    void close_socket() noexcept {
        if (sock_ >= 0) { ::close(sock_); sock_ = -1; }
    }
};

/* ====================================================================== *
   factory defined in can_backend.hpp
 * ====================================================================== */
std::unique_ptr<CanBackend> make_backend() {
    return std::make_unique<SocketCanBackend>();
}
