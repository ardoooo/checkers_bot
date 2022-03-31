#pragma once

enum Color {
    WHITE,
    BLACK
};

struct Unit {

    Unit(Color color, int x, int y): color(color), x(x), y(y) {};
    Unit() = default;
    Unit(const Unit&) = default;
    Unit(Unit&&) = default;
    Color color;
    bool is_king{false};
    int x;
    int y;
};