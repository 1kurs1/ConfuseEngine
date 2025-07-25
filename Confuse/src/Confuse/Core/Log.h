#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "Confuse/Core/Base.h"

namespace Confuse {
	class CONFUSE_API Log {
	public:
		static void init();
		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_clientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_coreLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;
	};
}

#define CE_CORE_TRACE(...) Confuse::Log::getCoreLogger()->trace(__VA_ARGS__)
#define CE_CORE_INFO(...) Confuse::Log::getCoreLogger()->info(__VA_ARGS__)
#define CE_CORE_WARN(...) Confuse::Log::getCoreLogger()->warn(__VA_ARGS__)
#define CE_CORE_ERROR(...) Confuse::Log::getCoreLogger()->error(__VA_ARGS__)
#define CE_CORE_FATAL(...) Confuse::Log::getCoreLogger()->critical(__VA_ARGS__)

#define CE_TRACE(...) Confuse::Log::getClientLogger()->trace(__VA_ARGS__)
#define CE_INFO(...) Confuse::Log::getClientLogger()->info(__VA_ARGS__)
#define CE_WARN(...) Confuse::Log::getClientLogger()->warn(__VA_ARGS__)
#define CE_ERROR(...) Confuse::Log::getClientLogger()->error(__VA_ARGS__)
#define CE_FATAL(...) Confuse::Log::getClientLogger()->critical(__VA_ARGS__)