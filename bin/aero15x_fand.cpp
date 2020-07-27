#include <aero15x/aero15x.hpp>
#include <aero15x/change_detector.hpp>
#include <aero15x/logging.hpp>
#include <aero15x/median_filter.hpp>

#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <thread>

#include <unistd.h>

using namespace std::string_literals;


/*  As far as I could tell, logging using systemd/journald is fairly
    straightforward -- just write to standard out and let systemd take care of
    it for you. Here we prefix the message with a number in arrow brackets to
    indicate to journald the level of the message. These don't quite map to
    the typical levels seen in development, e.g. fatal, error, warn, info,
    debug, trace. With systemd, there's no trace, there's an additional notice
    message, and additional messages specifc to the kernel. This has impacted
    the design of the internally used log levels -- the cardinal values have
    been mapped to a relevant journald level simply because I did not want to 
    have to write code to map levels from internal to journald.
*/
class journald_logger : public aero15x::log::logger
{
public:
    using aero15x::log::logger::logger;
protected:
    auto do_log(aero15x::log::level msg_level, std::string message) -> void
    {
        std::cout << "<" << static_cast<std::uint16_t>(msg_level) << ">";
        std::cout << message << std::endl;
    }
};



volatile std::atomic<bool> shutdown = false;

void signal_handler(int signal)
{
    shutdown = true;
}


auto main() -> int
{
    using namespace std::chrono_literals;	
    using namespace aero15x;

    log::make_and_set_logger<journald_logger>();
    log::set_level(log::level::debug);

    if (!is_aero15x()) {
        log::fatal("This product is not compatible, a Gigabyte Aero15x"
            " (P65Q) is required.");
        std::exit(1);
    }

    /*  Ordinarily I would have set upa compilation firewall between C/Linux and
        C++ code, but I thought it overkill for these applications. */
    if (::getuid() != 0) {
        log::fatal("You must be root to run this application");
        std::exit(1);
    }

    log::info("Aero 15x fan controller starting");



    /*  The reason why the filter has a window of three samples is to balance
        the removal of anomalous outliers, but still being able to respond
        quickly to actual changes in temperature. Typically, I observed
        these transients to last one sample. The downside of this is that for an
        actual change, we have to wait for an additional sample to confirm. A
        window larger than three would just increase the number of samples 
        required to detect a change. The filter is set to a high temperature to
        make sure that the state changes comply with starting at the highest
        state. If we didn't do this, the filter and the state would have to 
        converge on the correct solution, rather than in tandem.
    */
	auto median = median_filter<3>{{100, 100, 100}};

    auto ec = aero15x::ec_sys{};
    auto detect = aero15x::change_detector();
	auto change_speed = [&detect, &ec](
		auto cpu_temp,
		auto current_state)
	{
        aero15x::log::info("Changing speed to ", current_state.speed, ", "
            "for temp ", cpu_temp, "'c, ", 
            "next up ", current_state.up, ", ",
            "next down, ", current_state.down);
		ec.set_fan0_custom_speed(detect.get_current_state().speed);
        ec.set_fan1_custom_speed(detect.get_current_state().speed);
	};

    /*  If the daemon is run manually, intercepting SIGHUP allows the
        application to terminate cleanly and turn off the custom fan speeds if
        run in a terminal which is closed. */
	std::signal(SIGHUP, signal_handler);  
	std::signal(SIGINT, signal_handler);  
	std::signal(SIGTERM, signal_handler);


    ec.set_fan0_custom_speed(detect.get_current_state().speed);
    ec.set_fan1_custom_speed(detect.get_current_state().speed);
    ec.set_enable_custom_speed(true);



    while (!shutdown)
    {
       std::this_thread::sleep_for(1s);

        auto cpu_temp = ec.read_cpu_temp();
        auto filtered_temp = median.filter(static_cast<double>(cpu_temp));

        if (detect.change_needed(filtered_temp))
        {
            auto current_state = detect.get_current_state();
            change_speed(filtered_temp, current_state);
        }
    }

    aero15x::log::info("Shutting down, disabling custom speeds");
    ec.set_enable_custom_speed(false);
}
