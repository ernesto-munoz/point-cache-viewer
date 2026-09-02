#pragma once
#include <raylib.h>

constexpr float PIXELS_PER_METER = 100.0f;

// Metros (Box2D) -> Píxeles (Raylib)
inline float MetersToPixels(float meters) {
    return meters * PIXELS_PER_METER;
}

// Píxeles (Raylib) -> Metros (Box2D)
inline float PixelsToMeters(float pixels) {
    return pixels / PIXELS_PER_METER;
}

//struct Timer {
//    float interval;
//    float elapsed = 0.0f;
//
//    bool Tick(float dt) {
//        elapsed += dt;
//        if (elapsed >= interval) {
//            elapsed -= interval;
//            return true;
//        }
//        return false;
//    }
//};