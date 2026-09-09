#pragma once
#include <chrono>
#include <format>

namespace core {
    constexpr float PIXELS_PER_METER = 100.0f;

    // Metros (Box2D) -> Píxeles (Raylib)
    inline float MetersToPixels(float meters) {
        return meters * PIXELS_PER_METER;
    }

    // Píxeles (Raylib) -> Metros (Box2D)
    inline float PixelsToMeters(float pixels) {
        return pixels / PIXELS_PER_METER;
    }

    class ElapsedTime {
    public:
        ElapsedTime(std::string message) {
            start = std::chrono::steady_clock::now();
            message_ = message;
        }
        ~ElapsedTime() {
            auto stop = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            auto count = duration.count();  // need the lvalue
            std::cout << std::vformat(message_, std::make_format_args(count)) << std::endl;
        }
    private:
        std::chrono::steady_clock::time_point start;
        std::string message_;

    };
}
