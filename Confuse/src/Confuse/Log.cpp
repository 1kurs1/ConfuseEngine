#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Confuse{
    std::shared_ptr<spdlog::logger> Log::s_coreLogger;
    std::shared_ptr<spdlog::logger> Log::s_clientLogger;

    void Log::init(){
        spdlog::set_pattern("%^[%T] %n: %v%$");
        s_coreLogger = spdlog::stdout_color_mt("Confuse Engine");
        s_coreLogger->set_level(spdlog::level::trace);

        s_clientLogger = spdlog::stdout_color_mt("Application");
        s_clientLogger->set_level(spdlog::level::trace);
    }
}

// // core logger:
// #define CE_CORE_TRACE(...) ::Confuse::Log::getCoreLogger()->trace(__VA_ARGS__)
// #define CE_CORE_INFO(...) ::Confuse::Log::getCoreLogger()->info(__VA_ARGS__ )
// #define CE_CORE_WARN(...) ::Confuse::Log::getCoreLogger()->warn(__VA_ARGS__ )
// #define CE_CORE_ERROR(...) ::Confuse::Log::getCoreLogger()->error(__VA_ARGS__ )
// #define CE_CORE_FATAL(...) ::Confuse::Log::getCoreLogger()->fatal(__VA_ARGS__ )

// // client logger:
// #define CE_TRACE(...) ::Confuse::Log::getClientLogger()->trace(__VA_ARGS__ )
// #define CE_INFO(...) ::Confuse::Log::getClientLogger()->info(__VA_ARGS__ )
// #define CE_WARN(...) ::Confuse::Log::getClientLogger()->warn(__VA_ARGS__ )
// #define CE_ERROR(...) ::Confuse::Log::getClientLogger()->error(__VA_ARGS__)
// #define CE_FATAL(...) ::Confuse::Log::getClientLogger()->fatal(__VA_ARGS__)