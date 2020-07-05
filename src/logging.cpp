#include "logging.hpp"

#include <atomic>

using namespace aero15x;


namespace {

auto default_logger() -> log::logger*
{
    static auto logger = log::stdout_logger{log::level::info};
    return &logger;
}

auto get_logger_holder() -> std::atomic<log::logger*>&
{
    static auto holder = std::atomic<log::logger*>{default_logger()};
    return holder;
}

auto get_logger() -> log::logger&
{
    /*  this could cause a segmentation fault if user has set a
        custom logger and since deleted it */
    return *get_logger_holder().load();
}

}


auto aero15x::log::set_logger(logger* new_logger) -> void
{
    if (new_logger == nullptr) {
        new_logger = default_logger();
    }
    auto current = get_logger_holder().load();
    new_logger->set_log_level(current->get_log_level());
    get_logger_holder().exchange(new_logger);
}


auto aero15x::log::set_level(level new_level) -> void
{
    get_logger().set_log_level(new_level);
    log::debug("Log level now ", new_level);
}


namespace aero15x::log::internal
{

auto do_log(log::level message_level, std::string const & message) -> void
{
    get_logger().log(message_level, message);
}

}


using aero15x::log::logger;

logger::logger(level threshold)
    : m_level{threshold}
{}

auto logger::set_log_level(level level) -> void
{
    m_level = level;
}

auto logger::get_log_level() -> level
{
    return m_level;
}

auto logger::log(level msg_level, std::string message) -> void
{
    std::lock_guard<std::mutex> lock{m_mutex};
    if (msg_level > m_level) {
        return;
    }
    do_log(msg_level, message);
}

auto aero15x::log::operator<<(std::ostream& lhs, level rhs) -> std::ostream&
{
    switch(rhs) {
    case level::debug: lhs << "debug"; break;
    case level::info:  lhs << "info";  break;
    case level::warn:  lhs << "warn";  break;
    case level::fatal: lhs << "fatal"; break;
    case level::error: lhs << "error"; break;
    }
    return lhs;
}






