#pragma once

#include <filesystem>


namespace Grog::Filesystem {

    /**
     * @brief Get the current user's directory.
     */
    std::filesystem::path GetUserDirectoryPath();

    /**
     * @brief Get a suitable directory for user wide config.
     */
    std::filesystem::path GetUserConfigDirectoryPath();

    /**
     * @brief Get a suitable directory for system wide config.
     */
    std::filesystem::path GetSystemConfigDirectoryPath();

    /**
     * @brief Get a suitable directory for user wide data.
     */
    std::filesystem::path GetUserDataDirectoryPath();

    /**
     * @brief Get a suitable directory for user wide data.
     */
    std::filesystem::path GetSystemDataDirectoryPath();

    /**
     * @brief Geta a suitable directory for temp files.
     */
    std::filesystem::path GetTempDirectoryPath();

}