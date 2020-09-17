#include "Vessel.hpp"

Vessel::Vessel()
    : pos{0.0, 0.0}
    , v{0.0, 0.0}
{}

Vessel::Vessel(Coord2d pos, Coord2d v)
    : pos {pos}
    , v {v}
    , nav_params {.max_acc = 1.0, .target = nullptr}
{}

auto Vessel::get_pos() -> Coord2d
{
    return this->pos;
}

auto Vessel::set_nav_tgt(Coord2d* tgt) -> void
{
    this->nav_params.target = tgt;
}

auto Vessel::navigate() -> void
{
    // TODO
    this->pos.x += this->v.x;
    this->pos.y += this->v.y;
}

auto Vessel::update() -> void
{
    this->navigate();
}
