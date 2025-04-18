#pragma once
#include <cstdint>
#include <string_view>
#include <span>
#include <variant>
#include <optional>
#include <string>

namespace uds {

/* ------------------------------------------------------------------ *
   Supported scalar encodings                                          *
 * ------------------------------------------------------------------ */
enum class ScalarType {
    Float64,
    Float32,
    UInt32,  Int32,
    UInt16,  Int16,
    UInt8,   Int8
};

/* Try to map the third CSV column ("float32", "int16" …) to ScalarType */
std::optional<ScalarType> type_from_string(std::string_view);

/* ------------------------------------------------------------------ *
   Result container: one of the above types                            *
 * ------------------------------------------------------------------ */
using ScalarValue = std::variant<
        double,              // Float64
        float,               // Float32
        std::uint32_t,       // UInt32
        std::int32_t,        // Int32
        std::uint16_t,       // UInt16
        std::int16_t,        // Int16
        std::uint8_t,        // UInt8
        std::int8_t          // Int8
    >;

/* Parse the ISO‑TP response payload (big‑endian)                       *
 * Return std::nullopt on size/type mismatch                           */
std::optional<ScalarValue> parse_payload(std::span<const std::uint8_t>,
                                         ScalarType);

/* Convenience helpers ------------------------------------------------ */
double           to_double (const ScalarValue&);            // always returns NAN‑safe double
std::string      format     (const ScalarValue& v,
                             ScalarType t,
                             std::string_view mode = "dec"); // "dec" | "hex" | "bin"

} // namespace uds
