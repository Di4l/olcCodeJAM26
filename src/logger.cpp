//-----------------------------------------------------------------------------
#include "logger.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>
//-----------------------------------------------------------------------------
using namespace codejam26;
//-----------------------------------------------------------------------------
named_loggers Logger::m_namedLoggers{};
shared_sinks  Logger::m_sharedSinks{};
std::mutex    Logger::m_sinkMutex{};
std::mutex    Logger::m_loggersMutex{};
//-----------------------------------------------------------------------------

spdlog::logger& Logger::get(std::string const& category)
{
    //-- Make sure main logger is created
    init();

    {
        std::lock_guard<std::mutex> lock(m_loggersMutex);

        auto it = m_namedLoggers.find(category);
        if (it != m_namedLoggers.end())
            return *(it->second);
    }

    // Si no existe, lo creamos
    return *addLogger(category);
}
//-----------------------------------------------------------------------------

void Logger::addSink(shared_sink sink)
{
    //-- Make sure the shared sink is created
    init();

    std::lock_guard<std::mutex> lock(m_sinkMutex);

    auto it = std::find(m_sharedSinks->begin(), m_sharedSinks->end(), sink);
    if (it == m_sharedSinks->end())
    {
        m_sharedSinks->push_back(sink);
        // Update all logger's sinks
        std::lock_guard<std::mutex> map_lock(m_loggersMutex);
        for (auto& [_, logger] : m_namedLoggers)
            logger->sinks().push_back(sink);
    }
}
//-----------------------------------------------------------------------------

void Logger::init()
{
    if(!m_sharedSinks)
    {
        //-- Create shared vector of sinks for all our loggers
        m_sharedSinks = std::make_shared<std::vector<shared_sink>>();
        //-- Create a standard default sink
        auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        sink->set_pattern("[%T] [%n] [%^%l%$] %v"); // Time, logger name, level, message
        //-- Add the sink to the vector
        m_sharedSinks->push_back(sink);
    }
}
//-----------------------------------------------------------------------------

shared_logger Logger::addLogger(std::string const& category)
{
    std::lock_guard<std::mutex> lock_log(m_loggersMutex);

    auto it = m_namedLoggers.find(category);
    if (it != m_namedLoggers.end())
        return it->second;

    std::lock_guard<std::mutex> lock_sink(m_sinkMutex);

    auto logger = std::make_shared<spdlog::logger>(category, m_sharedSinks->begin(), m_sharedSinks->end());
    // Set runtime logger level according to the compile-time active level
    // If SPDLOG_ACTIVE_LEVEL indicates debug or lower, enable debug; otherwise use info.
#if defined(SPDLOG_ACTIVE_LEVEL)
#  if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_DEBUG
    logger->set_level(spdlog::level::debug);
#  else
    logger->set_level(static_cast<spdlog::level::level_enum>(SPDLOG_ACTIVE_LEVEL));
#  endif
#else
  // Fallback: tie to NDEBUG (debug builds -> debug level)
#  ifdef NDEBUG
    logger->set_level(spdlog::level::info);
#  else
    logger->set_level(spdlog::level::debug);
#  endif
#endif
    logger->flush_on(spdlog::level::err);

    spdlog::register_logger(logger);
    if(m_namedLoggers.size() == 0)
        spdlog::set_default_logger(logger);
    m_namedLoggers[category] = logger;

    return logger;
}
//-----------------------------------------------------------------------------
