#pragma once

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <utility>

/*
    Embedded Controller Class for Aero 15x Laptop
    =============================================

    Implementation inspired from https://github.com/jertel/p37-ec

    The initial version of this class initially used a generic class 
    that had "register" information passed into it. However, this caused
    some issues and was very difficult to test. It also eliminates a whole
    class of user errors -- using the wrong types.

    There are a few more functions available through the ec system 

    As far as I understand it, `std::launder` is a compilation barrier;
    it notifies the compiler not to optimise anything based upon reading
    or writing arbitrary data from pointers.
*/

namespace aero15x {

constexpr static auto fan_speed_max = 229ull;
constexpr static auto fan_speed_min = 90ull; /* This is arbitrary */

auto is_aero15x() -> bool;


class ec_sys
{
public:
    ec_sys();
    ec_sys(std::string const & stream);

    template <typename T>
    auto write(std::size_t address, T const & t) -> void
    {
        m_file.seekp(address);
        m_file.write(reinterpret_cast<char const*>(std::launder(&t)),
            sizeof(t));
    }

    template <typename T>
    auto read(std::size_t address) -> T
    {
        T t{};
        m_file.seekg(address);
        m_file.read(reinterpret_cast<char*>(std::launder(&t)), sizeof(t));
        return t;
    }

    auto set_fan0_custom_speed(std::uint8_t speed) -> void;
    auto set_fan1_custom_speed(std::uint8_t speed) -> void;
    auto read_fan0_custom_speed() -> std::uint8_t;
    auto read_fan1_custom_speed() -> std::uint8_t;
    auto read_fan0_speed() -> std::uint16_t;
    auto read_fan1_speed() -> std::uint16_t;
    auto read_cpu_temp() -> std::uint8_t;
    auto read_gpu_temp() -> std::uint8_t;
    auto read_mlb_temp() -> std::uint8_t;
    auto get_enable_custom_speed() -> bool;
    auto set_enable_custom_speed(bool enable) -> void;

private:
    std::fstream m_file;
};

}