#include <aero15x/aero15x.hpp>
#include <aero15x/logging.hpp>

#include <cstdlib>

#include <unistd.h>


auto main() -> int
{    
    if (!aero15x::is_aero15x()) {
        aero15x::log::fatal("This product is not compatible, a Gigabyte Aero15x"
            " (P65Q) is required.");
        std::exit(1);
    }
    if (::getuid() != 0) {
        aero15x::log::fatal("You must be root to run this application");
        std::exit(1);
    }
    auto ec = aero15x::ec_sys{};
    aero15x::log::info("Setting fans to maximum");
    ec.set_enable_custom_speed(true);
    ec.set_fan0_custom_speed(aero15x::fan_speed_max);
    ec.set_fan1_custom_speed(aero15x::fan_speed_max);
}
