#include "../include/udscom/parser.hpp"
#include <cstring>
#include <algorithm>     // std::reverse_copy
#include <bit>
#include <charconv>
#include <sstream>
#include <iomanip>

namespace {

template<typename T>
T read_be(const std::uint8_t* p) {              // big‑endian → host
#if defined(__cpp_lib_byteswap) && __cpp_lib_byteswap >= 202110L
    return std::byteswap(*reinterpret_cast<const T*>(p));
#else
    T temp{};
    std::reverse_copy(p, p + sizeof(T),
                      reinterpret_cast<std::uint8_t*>(&temp));
    return temp;
#endif
}

} // unnamed namespace

namespace uds {

/* ------------------------------------------------- type_from_string */
std::optional<ScalarType> type_from_string(std::string_view s) {
    auto eat = [](std::string_view in){
        std::string low(in);
        std::transform(low.begin(), low.end(), low.begin(), ::tolower);
        return low;
    };
    std::string ls = eat(s);
    if      (ls == "float64" || ls == "double") return ScalarType::Float64;
    else if (ls == "float32" || ls == "single") return ScalarType::Float32;
    else if (ls == "uint32")                    return ScalarType::UInt32;
    else if (ls == "int32")                     return ScalarType::Int32;
    else if (ls == "uint16")                    return ScalarType::UInt16;
    else if (ls == "int16")                     return ScalarType::Int16;
    else if (ls == "uint8")                     return ScalarType::UInt8;
    else if (ls == "int8")                      return ScalarType::Int8;
    return std::nullopt;
}

/* --------------------------------------------------- parse_payload */
std::optional<ScalarValue>
parse_payload(std::span<const std::uint8_t> p, ScalarType t) {

    if ( (t==ScalarType::Float64 || t==ScalarType::UInt32 || t==ScalarType::Int32)
         && p.size() < 4 ) return std::nullopt;
    if ( (t==ScalarType::Float64) && p.size() < 8 ) return std::nullopt;
    if ( (t==ScalarType::UInt16 || t==ScalarType::Int16) && p.size() < 2 )
         return std::nullopt;

    switch (t) {
        case ScalarType::Float64:  {
            auto v = read_be<std::uint64_t>(p.data());
            double d;
            std::memcpy(&d, &v, sizeof d);
            return d;
        }
        case ScalarType::Float32:  {
            auto v = read_be<std::uint32_t>(p.data());
            float f;
            std::memcpy(&f, &v, sizeof f);
            return f;
        }
        case ScalarType::UInt32:   return read_be<std::uint32_t>(p.data());
        case ScalarType::Int32:    return read_be<std::int32_t >(p.data());
        case ScalarType::UInt16:   return read_be<std::uint16_t>(p.data());
        case ScalarType::Int16:    return read_be<std::int16_t >(p.data());
        case ScalarType::UInt8:    return p[0];
        case ScalarType::Int8:     return static_cast<std::int8_t>(p[0]);
    }
    return std::nullopt;   // unreachable, but silences warnings
}

/* ------------------------------------------------------- to_double */
double to_double(const ScalarValue& v) {
    return std::visit([](auto&& arg)->double { return static_cast<double>(arg); }, v);
}

/* -------------------------------------------------------- format  */
std::string format(const ScalarValue& v, ScalarType t, std::string_view mode){
    std::ostringstream oss;

    auto print_integral = [&](auto integral){
        using U = std::make_unsigned_t<decltype(integral)>;
        U u = static_cast<U>(integral);
        if (mode == "hex") {
            oss << "0x" << std::hex << std::uppercase << +u;
        } else {                    // binary
            for (int i = sizeof(U)*8-1; i>=0; --i)
                oss << ((u>>i)&1);
            oss << 'b';
        }
    };

    if (mode == "hex" || mode == "bin") {
        std::visit([&](auto arg){
            if constexpr (std::is_integral_v<decltype(arg)>)
                print_integral(arg);
            else
                oss << arg;                           // fall back to decimal
        }, v);
    } else {
        std::visit([&](auto arg){ oss << +arg; }, v); // decimal
    }
    return oss.str();
}


} // namespace uds