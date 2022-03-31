#pragma once

#include "unit.h"
#include "../src/helpers.cpp"
#include <vector>
#include <memory>
#include <set>
#include <cmath>


class Field {
public:

    Field();

    // если перемещение шашки из позиции (start_x, start_y) в (end_x, end_y) возможно, то возвращает true,
    // иначе false
    bool CheckMove(int start_x, int start_y, int end_x, int end_y);

    // перемещает шашку из позиции (start_x, start_y) в (end_x, end_y), если возможно,
    // и возвращает true, иначе fales
    bool MoveUnit(int start_x, int start_y, int end_x, int end_y);

    // если на позиции (x, y)  стоит шашка, то убивает её и возвращает true, иначе false
    bool KillUnit(int x, int y);

    // проверяет наличие ходов атаки, 
    // если такие есть, возвращает true и убивает максимальное количество шашек, иначе false
    bool CheckStepKill();

    // проверяет наличие хода, если ход есть, совершает его и возвращает true, 
    // приоритетным считается ход, который не подставит под удар шашку
    // гарантируется, что при наличии приоритетных ходов, исполнится приоритетный
    bool CheckSafeMove();

    // проверяет наличие произвольного хода
    // если ход есть, совершает его и возвращает true, иначе false
    bool CheckStep();

    void SetColor(Color color);

    void InvertColor();

private:

    const int FIELD_SIZE = 8;

    std::vector<std::vector<std::unique_ptr<Unit>>> field;
    std::set<std::pair<int, int>> white_positions;
    std::set<std::pair<int, int>> black_positions;

    Color current_color{Color::WHITE};

};