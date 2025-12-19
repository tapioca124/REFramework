#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "RETypes.hpp"
#include "REGlobals.hpp"
#include "ResourceManager.hpp"

#include "SDK.hpp"

namespace reframework {
void initialize_sdk() {
    if (spdlog::default_logger() == nullptr) {
        spdlog::set_default_logger(spdlog::basic_logger_mt("REFrameworkSDK", "reframework_sdk_log.txt", true));
    }

    spdlog::info("Initializing REFramework SDK...");
    spdlog::info("REFramework SDK: https://github.com/praydog/REFramework");

    reframework::get_types();
    reframework::get_globals();

    utility::re_managed_object::detail::resolve_add_ref();
    utility::re_managed_object::detail::resolve_release();

    // Only resolve create_resource eagerly; create_userdata is resolved lazily (and may be unavailable per-game/version).
    sdk::ResourceManager::update_pointers();
}
}
