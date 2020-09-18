#ifndef SRC_VESSEL_HPP_
#define SRC_VESSEL_HPP_

#include "helpers.hpp"

enum Nav_mode {
    NAV_idle = 0,
    NAV_approach,
    NAV_keep_distance,
    NAV_increase_distance,
    NAV_orbit
};

struct Navigation_params final {
    Nav_mode mode;
    double max_acc; // max acceleration in meters per second
    Coord2d* target; // in the future will probably be a pointer to object
};

class Vessel final {
public:
    Vessel();
    Vessel(Coord2d pos, Coord2d v);

    auto get_pos() -> Coord2d;

    auto set_nav_tgt(Coord2d* tgt) -> void;
    auto set_nav_mode(Nav_mode mode) -> void;

    auto navigate() -> void;
    auto nav_approach() -> void;
    auto update() -> void;

private:
    Coord2d pos;
    Coord2d v;
    Navigation_params nav_params;
};

#endif // SRC_VESSEL_HPP_
