#include "aero15x.hpp"
#include "logging.hpp"

#include <cstddef>
#include <cstdint>
#include <string>

#include <endian.h>



namespace {

using namespace std::string_literals;

constexpr static auto address_cpu_temp = 0x60;
constexpr static auto address_gpu_temp = 0x61;
constexpr static auto address_mlb_temp = 0x62;
constexpr static auto address_fan0_speed = 0xfc;
constexpr static auto address_fan1_speed = 0xfe;
constexpr static auto address_fan0_custom_speed = 0xb0;
constexpr static auto address_fan1_custom_speed = 0xb1;
constexpr static auto address_enable_custom_speed = 0x06;

static auto ec_sys_file = "/sys/kernel/debug/ec/ec0/io"s;
static auto vendor_file = "/sys/devices/virtual/dmi/id/sys_vendor"s;
static auto product_file = "/sys/devices/virtual/dmi/id/product_name"s;
static auto required_vendor = "GIGABYTE"s;
static auto required_product = "P65Q"s;

}

auto aero15x::is_aero15x() -> bool 
{
    auto vendor_string = std::string{};
    auto product_string = std::string{};
    auto vendor_stream = std::ifstream{vendor_file};
    auto product_stream = std::ifstream{product_file};

    vendor_stream >> vendor_string;
    product_stream >> product_string;
    if (product_stream.fail() || vendor_stream.fail()) {
        return false;
    }

    aero15x::log::debug("Vendor is ", vendor_string, ", product is ",
        product_string);

    return (vendor_string == required_vendor) &&
        (product_string == required_product);
}


aero15x::ec_sys::ec_sys(std::string const & file)
    : m_file{file, std::ios::binary}
{}

aero15x::ec_sys::ec_sys()
    : ec_sys{ec_sys_file}
{}


auto aero15x::ec_sys::set_fan0_custom_speed(std::uint8_t speed) -> void
{
    if (speed > fan_speed_max) {
        return;
    }
    write(address_fan0_custom_speed, speed);
}

auto aero15x::ec_sys::set_fan1_custom_speed(std::uint8_t speed) -> void
{
    if (speed > fan_speed_max) {
        return;
    }
    write(address_fan1_custom_speed, speed);
}

auto aero15x::ec_sys::read_fan0_custom_speed() -> std::uint8_t
{
    return read<std::uint8_t>(address_fan0_custom_speed);
}

auto aero15x::ec_sys::read_fan1_custom_speed() -> std::uint8_t
{
    return read<std::uint8_t>(address_fan1_custom_speed);
}

auto aero15x::ec_sys::read_fan0_speed() -> std::uint16_t
{
    return htobe16(read<std::uint16_t>(address_fan0_speed));
}

auto aero15x::ec_sys::read_fan1_speed() -> std::uint16_t
{
    return htobe16(read<std::uint16_t>(address_fan1_speed));
}

auto aero15x::ec_sys::read_cpu_temp() -> std::uint8_t
{
    return read<std::uint8_t>(address_cpu_temp);
}

auto aero15x::ec_sys::read_gpu_temp() -> std::uint8_t
{
    return read<std::uint8_t>(address_gpu_temp);
}

auto aero15x::ec_sys::read_mlb_temp() -> std::uint8_t
{
    return read<std::uint8_t>(address_mlb_temp);
}

auto aero15x::ec_sys::get_enable_custom_speed() -> bool
{
    auto val = read<std::uint8_t>(address_enable_custom_speed);
    return static_cast<bool>(val);
}

auto aero15x::ec_sys::set_enable_custom_speed(bool enable) -> void
{
    if (enable) {
        write(address_enable_custom_speed, std::uint8_t{0x1e});
    }
    else {
        write(address_enable_custom_speed, std::uint8_t{0x00});
    }
}