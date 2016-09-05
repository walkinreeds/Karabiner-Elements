#define CATCH_CONFIG_MAIN
#include "../../vendor/catch/catch.hpp"

#include "configuration_core.hpp"
#include <iostream>
#include <spdlog/spdlog.h>

class logger final {
public:
  static spdlog::logger& get_logger(void) {
    static std::shared_ptr<spdlog::logger> logger;
    if (!logger) {
      logger = spdlog::stdout_logger_mt("configuration_core", true);
    }
    return *logger;
  }
};

TEST_CASE("valid") {
  configuration_core configuration(logger::get_logger(), "json/example.json");

  {
    std::vector<std::pair<krbn::key_code, krbn::key_code>> expected{
        std::make_pair(krbn::key_code(57), krbn::key_code(42)),
        std::make_pair(krbn::key_code(41), krbn::key_code(44)),
    };
    REQUIRE(configuration.get_current_profile_simple_modifications() == expected);
  }
  {
    std::vector<std::pair<krbn::key_code, krbn::key_code>> expected{
        std::make_pair(krbn::key_code::f1, krbn::key_code::vk_consumer_brightness_down),
        std::make_pair(krbn::key_code::f10, krbn::key_code::vk_consumer_mute),
        std::make_pair(krbn::key_code::f11, krbn::key_code::vk_consumer_sound_down),
        std::make_pair(krbn::key_code::f12, krbn::key_code::vk_consumer_sound_up),
        std::make_pair(krbn::key_code::f2, krbn::key_code::vk_consumer_brightness_up),
        std::make_pair(krbn::key_code::f3, krbn::key_code::vk_mission_control),
        std::make_pair(krbn::key_code::f4, krbn::key_code::vk_launchpad),
        std::make_pair(krbn::key_code::f5, krbn::key_code::vk_consumer_illumination_down),
        std::make_pair(krbn::key_code::f6, krbn::key_code::vk_consumer_illumination_up),
        std::make_pair(krbn::key_code::f7, krbn::key_code::vk_consumer_previous),
        std::make_pair(krbn::key_code::f8, krbn::key_code::vk_consumer_play),
        std::make_pair(krbn::key_code::f9, krbn::key_code::vk_consumer_next),
    };
    REQUIRE(configuration.get_current_profile_fn_function_keys() == expected);
  }

  REQUIRE(configuration.is_loaded() == true);
}

TEST_CASE("broken.json") {
  configuration_core configuration(logger::get_logger(), "json/broken.json");

  std::vector<std::pair<krbn::key_code, krbn::key_code>> expected;
  REQUIRE(configuration.get_current_profile_simple_modifications() == expected);
  REQUIRE(configuration.is_loaded() == false);
}

TEST_CASE("invalid_key_code_name.json") {
  configuration_core configuration(logger::get_logger(), "json/invalid_key_code_name.json");

  std::vector<std::pair<krbn::key_code, krbn::key_code>> expected{
      std::make_pair(krbn::key_code(41), krbn::key_code(44)),
  };
  REQUIRE(configuration.get_current_profile_simple_modifications() == expected);
  REQUIRE(configuration.is_loaded() == true);
}
