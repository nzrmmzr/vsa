include(FetchContent)

include(cmake/imgui.cmake)

FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG        55f93686c01528224f448c19128836e7df245f72
)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG        a532f5b1cf27d6a3c099437e6959cf7e398a0a67 # v1.0.2
)

FetchContent_Declare(
        logger
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        486b55554f11c9cccc913e11a87085b2a91f706f
)

FetchContent_MakeAvailable(json glm logger)

add_library(json::json ALIAS nlohmann_json)