module;

#include <map>
#include <mutex>
#include <random>
#include <shared_mutex>
#include <string>

export module Game.Rand;

export class Rand final {
public:
  Rand() = delete;

  static void add_uniform_real_distribution(const std::string &name,
                                            float lower_bound,
                                            float upper_bound) {
    std::unique_lock<std::shared_mutex> lock{sync_object};
    uniform_real_distro_map.insert_or_assign(
        name, std::uniform_real_distribution<float>{lower_bound, upper_bound});
  }

  static float get_uniform_real() {
    return default_uniform_real_distro(get_engine());
  }

  static float get_uniform_real(const std::string &name) {
    std::shared_lock<std::shared_mutex> lock{sync_object};

    if (const auto it = uniform_real_distro_map.find(name);
        it != uniform_real_distro_map.end()) {
      return it->second(get_engine());
    }

    return get_uniform_real();
  }

private:
  static std::mt19937 &get_engine() {
    thread_local std::random_device rd;
    thread_local std::mt19937 engine{rd()};
    return engine;
  }

  static inline std::shared_mutex sync_object;
  static inline std::map<std::string, std::uniform_real_distribution<float>>
      uniform_real_distro_map;
  static inline std::uniform_real_distribution<float>
      default_uniform_real_distro{0.0f, 1.0f};
};