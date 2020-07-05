#pragma once

#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <mutex>


namespace aero15x::log {

/*  To keep things more simple, not including a trace level as journald does not
	have this, and I didn't want to map journald levels to more "common" ones.
	Doable though. */
enum class level : std::uint16_t
{
    fatal = 2, // jounrnald "crit" message
    error = 3,
    warn  = 4, 
    info  = 6,
    debug = 7,
};




namespace internal {

template <typename ... Args>
static auto make_string(Args && ... args) -> std::string
{
	/* 	We can't, to my knowledge, use `std::stringstream` as an initialiser in 
		a fold expression, because `operator<<()` returns a `std::ostream`,
		which will be the eventual result of the expression, which obviously
		does not have a `str()` method */

    auto stream = std::stringstream{};
    (stream << ... << (args));
    return stream.str();
}

auto do_log(level message_level, std::string const & message) -> void;

}



class logger
{
public:
    logger(level threshold);
    ~logger() = default;
    auto set_log_level(level level) -> void;
    auto get_log_level() -> level;
    auto log(level msg_level, std::string message) -> void;

protected:
    virtual auto do_log(level msg_level, std::string message) -> void = 0;

private:
    level m_level;
    std::mutex m_mutex;
};


auto operator<<(std::ostream& lhs, level rhs) -> std::ostream&;

/* Any new logger provided here must not be used */
auto set_logger(logger* new_logger) -> void;
auto set_level(level new_level) -> void;




class stdout_logger final : public logger
{
public:
    using logger::logger;
private:
    auto do_log(level msg_level, std::string message) -> void
    {
        constexpr static auto max_level_width = 5;
        std::cout << "[" << std::setw(max_level_width) << std::left 
            << msg_level << "]: " << message << std::endl;
    }
};




template <typename ... Args>
auto info(Args && ... args) -> void
{
    internal::do_log(level::info, 
        internal::make_string(std::forward<Args>(args)...));
}    

template <typename ... Args>
auto debug(Args && ... args) -> void
{
    internal::do_log(level::debug, 
        internal::make_string(std::forward<Args>(args)...));
} 

template <typename ... Args>
auto error(Args && ... args) -> void
{
    internal::do_log(level::error, 
        internal::make_string(std::forward<Args>(args)...));
}

template <typename ... Args>
auto warn(Args && ... args) -> void
{
    internal::do_log(level::warn, 
        internal::make_string(std::forward<Args>(args)...));
}

template <typename ... Args>
auto fatal(Args && ... args) -> void
{
    internal::do_log(level::fatal, 
        internal::make_string(std::forward<Args>(args)...));
}


}
