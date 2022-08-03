#include "Vessel.hpp"

#include "logs.hpp"

Vessel::Vessel()
    : pos{0.0, 0.0}
    , v{0.0, 0.0}
{}

Vessel::Vessel(Coord2d pos, Coord2d v)
    : pos {pos}
    , v {v}
    , nav_params {.mode = NAV_idle, .max_acc = 0.1, .target = nullptr}
{}

auto Vessel::get_pos() -> Coord2d
{
    return this->pos;
}

auto Vessel::set_nav_tgt(Coord2d* tgt) -> void
{
    DBG(9, "setting nav target to ", tgt->x, "x", tgt->y);
    this->nav_params.target = tgt;
}

auto Vessel::set_nav_mode(Nav_mode mode) -> void
{
    this->nav_params.mode = mode;
}


auto Vessel::navigate() -> void
{
    switch (this->nav_params.mode) {
        case NAV_approach:
            this->nav_approach();
            break;
        default:
            break;
    }
}

auto Vessel::nav_approach() -> void
{
    if (!this->nav_params.target) {
        DBG(1, "nav has no target");
        this->nav_params.mode = NAV_idle;

        return;
    }

    double dist_x {this->nav_params.target->x - this->get_pos().x};
    double dist_y {this->nav_params.target->y - this->get_pos().y};

    double abs_dist_x = abs(dist_x);
    double abs_dist_y = abs(dist_y);

    double dir_x {abs_dist_x / (abs_dist_x + abs_dist_y)};
    if (dir_x > 1.0) { dir_x = 1.0 / dir_x; }
    double dir_y {1.0 - dir_x};
    if (dist_x < 0.0) { dir_x = -dir_x; }
    if (dist_y < 0.0) { dir_y = -dir_y; }

    double acc {this->nav_params.max_acc};
    double acc_x = acc * dir_x;
    double acc_y = acc * dir_y;

    double total_velocity {abs(this->v.x) + abs(this->v.y)};
    double braking_time {total_velocity / this->nav_params.max_acc};
    double braking_dist {total_velocity * braking_time};
    double distance_to_tgt = abs_dist_x + abs_dist_y;

    DBG(1, "distance to target: ", distance_to_tgt);
    DBG(1, "braking distance: ", braking_dist);
    DBG(1, "acceleration: x/y ", acc_x, "/", acc_y);

    this->v.x += acc_x;
    this->v.y += acc_y;
}


auto Vessel::update() -> void
{
    this->navigate();

    this->pos.x += this->v.x;
    this->pos.y += this->v.y;
}
