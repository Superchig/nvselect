#include <filesystem>
#include <iostream>
#include "util.h"
#include "toml11/toml.hpp"

std::vector<std::pair<std::string, std::string>> readConfig()
{
    const std::string HOME_DIR{getenv("HOME")};
    const std::string CONFIG_DIR{HOME_DIR + "/.config/nvselect"};
    const std::string CONFIG_FILE{CONFIG_DIR + "/nvselect.toml"};

    if (!std::filesystem::exists(CONFIG_DIR))
    {
        std::filesystem::create_directories(CONFIG_DIR);
    }

    std::vector<std::pair<std::string, std::string>> choices;
    if (std::filesystem::exists(CONFIG_FILE))
    {
        const toml::value config = toml::parse(CONFIG_FILE);
        const auto& choicesAot = toml::find<std::vector<toml::table>>(config, "choices");

        for (toml::table choice : choicesAot)
        {
          const std::string display = choice.at("display").as_string();

          const bool hasScript = choice.find("script") != choice.end();
          const std::string script = (hasScript) ? choice.at("script").as_string() : "";

          choices.push_back(std::pair<std::string, std::string>(display, script));
        }
    }
    else
    {
        // Default options
        choices.push_back(std::pair<std::string, std::string>("NVIDIA", "/usr/bin/nvidia-xrun"));
        choices.push_back(std::pair<std::string, std::string>("Intel", "/home/chiggie/dotfiles/msi_gs65/startx-wrapper"));
    }

    return choices;
}