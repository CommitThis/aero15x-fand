#include <aero15x/aero15x.hpp>
#include <aero15x/logging.hpp>

#include <unistd.h>


auto main() -> int
{   
    if (!aero15x::is_aero15x()) {
        aero15x::log::fatal("This product is not compatible, a Gigabyte Aero15x"
            " (P65Q) is required.");
        ::exit(1);
    }
    if (::getuid() != 0) {
        aero15x::log::fatal("You must be root to run this application");
        ::exit(1);
    }
    auto ec = aero15x::ec_sys{};
    ec.set_enable_custom_speed(false);
    aero15x::log::info("Disabling custom speeds");
}
