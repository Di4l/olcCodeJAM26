//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#include <spdlog/spdlog.h>

#include <vector>
#include <mutex>
#include <memory>
#include <unordered_map>
//-----------------------------------------------------------------------------
#define LOGGER  codejam26::Logger::get()
//-----------------------------------------------------------------------------

namespace codejam26
{
    //-------------------------------------------------------------------------
    using shared_logger = std::shared_ptr<spdlog::logger>;
    using named_loggers = std::unordered_map<std::string, shared_logger>;
    using shared_sink   = std::shared_ptr<spdlog::sinks::sink>;
    using shared_sinks  = std::shared_ptr<std::vector<shared_sink>>;
    //-----------------------------------------------------------------------------

    class Logger
    {
    public:
        virtual ~Logger() = default;
        // Prohibe la copia
        Logger(Logger const&) = delete;
        Logger& operator=(Logger const&) = delete;

        static spdlog::logger& get(std::string const& category = { "codejam26" });

        static void addSink(shared_sink sink);

    private:
        Logger() = default;

        static void init();

        static shared_logger addLogger(std::string const& category);

        static named_loggers m_namedLoggers;
        static shared_sinks  m_sharedSinks;
        static std::mutex    m_sinkMutex;
        static std::mutex    m_loggersMutex;
    };
    //-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
