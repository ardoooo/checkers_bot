#include "../include/field.h"
#include <algorithm>
#include <memory>
#include <vector>

Field::Field() {
    for (int i = 0; i < FIELD_SIZE; ++i) {
        field.emplace_back(std::vector<std::unique_ptr<Unit>>(FIELD_SIZE));
    }
    for (int i = 0; i < FIELD_SIZE; ++i) {
        if (i % 2 == 0) {
            white_positions.emplace(0, i);
            white_positions.emplace(2, i);
            black_positions.emplace(6, i);
        } else {
            white_positions.emplace(1, i);
            black_positions.emplace(5, i);
            black_positions.emplace(7, i);
        }
    }

    for (auto pos: white_positions) {
        field[pos.first][pos.second] = std::make_unique<Unit>(Color::WHITE, pos.first, pos.second);
    }

    for (auto pos: black_positions) {
        field[pos.first][pos.second] = std::make_unique<Unit>(Color::BLACK, pos.first, pos.second);
    }

}

bool Field::CheckMove(int start_x, int start_y, int end_x, int end_y) {

    // проверка корректности позиций
    if (start_x < 0 || start_x >= FIELD_SIZE || start_y < 0 || start_y >= FIELD_SIZE) {
        return false;
    }

    if (end_x < 0 || end_x >= FIELD_SIZE || end_y < 0 || end_y >= FIELD_SIZE) {
        return false;
    }

    // есть ли шашка на начальной позиции
    if (field[start_x][start_y] == nullptr) {
        return false;
    }

    // есть ли шашка на конечной позиции
    if (field[end_x][end_y] != nullptr) {
        return false;
    }

    // проверка для дамок и обычных шашек разная
    if (!field[start_x][start_y]->is_king) {
        if (field[start_x][start_y]->color == Color::WHITE) {
            return (std::abs(start_x - end_x) == 1 && end_x - start_x == 1);
        } else {
            return (std::abs(start_x - end_x) == 1 && end_x - start_x == -1);
        }
    } else {
        // проверка, что позиции на одной диагонали
        if (std::abs(start_x - end_x) != std::abs(start_y - end_y)) {
            return false;
        }

        // проверим, что между позициями нет других шашек
        int step_x = (start_x - end_x > 0 ? -1 : 1);
        int step_y = (start_y - end_y > 0 ? -1 : 1);

        for (int x = start_x + step_x, y = start_y + step_y; x < end_x; x += step_x, y += step_y) {
            if (field[x][y] != nullptr) {
                return false;
            }
        }
        return true;
    }
}

bool Field::MoveUnit(int start_x, int start_y, int end_x, int end_y) {
    if (!CheckMove(start_x, start_y, end_x, end_y)) {
        return false;
    }
    if (field[start_x][start_y]->color == Color::WHITE) {
        white_positions.erase({start_x, start_y});
        white_positions.insert({end_x, end_y});
    } else {
        black_positions.erase({start_x, start_y});
        black_positions.insert({end_x, end_y});
    }
    std::swap(field[start_x][start_y], field[end_x][end_y]);
    return true;
}

bool Field::KillUnit(int x, int y) {
    if (field[x][y] == nullptr) {
        return false;
    }
    if (field[x][y]->color == Color::WHITE) {
        white_positions.erase({x, y});
    } else {
        black_positions.erase({x, y});
    }
    delete field[x][y].release();
    return true;
}

bool Field::CheckStepKill() {
    int killed_max = 0; // максимальное число съеденных
    std::vector<std::pair<int, int>> killed_max_list; // список съеденых при максимуме
    int x_start_max = 0, y_start_max = 0; // начальные координаты шашки, которую двигаем
    int x_final_max = 0, y_final_max = 0; // итоговые координаты текущей шашки, которую двигаем
    std::set<std::pair<int, int>> *current_set = &white_positions;
    if (current_color == Color::BLACK) {
        current_set = &black_positions;
    }
    for (const auto &checker : *current_set) {
        int x = checker.first;
        int y = checker.second;
        std::vector<std::vector<bool>> used(FIELD_SIZE, std::vector<bool>(FIELD_SIZE, false));
        std::vector<std::vector<bool>> eaten(FIELD_SIZE, std::vector<bool>(FIELD_SIZE, false));
        KillDFS(used, eaten, field, current_color, x, y, FIELD_SIZE);
        std::vector<std::pair<int, int>> fields_to_kill; // все поля, которые можем съесть
        for (int i = 0; i < FIELD_SIZE; ++i) {
            for (int j = 0; j < FIELD_SIZE; ++j) {
                if (eaten[i][j]) {
                    fields_to_kill.emplace_back(i, j);
                }
            }
        }
        // теперь у нас есть список всех клеток, с шашками, которые можем съесть за текущий ход
        std::sort(fields_to_kill.begin(), fields_to_kill.end()); // сортируем для next_permutation
        int killed_tmp;
        std::vector<std::pair<int, int>> killed_list_tmp;
        int x_final_tmp, y_final_tmp;
        do {
            killed_tmp = TryPerformKill(field, x, y, fields_to_kill, x_final_tmp, y_final_tmp,
                                        killed_list_tmp, FIELD_SIZE);
            if (killed_tmp > killed_max) {
                killed_max = killed_tmp;
                x_final_max = x_final_tmp;
                y_final_max = y_final_tmp;
                killed_max_list = killed_list_tmp;
                x_start_max = x;
                y_start_max = y;
            }
            killed_list_tmp.clear();
        } while (std::next_permutation(fields_to_kill.begin(), fields_to_kill.end()));
    }
    for (const auto &killed : killed_max_list) {
        KillUnit(killed.first, killed.second);
    }
    if (killed_max > 0) {
        std::swap(field[x_start_max][y_start_max],
                  field[x_final_max][y_final_max]); // передвигаем шашку
        return true;
    }
    return false;
}

bool Field::CheckSafeMove() {
    std::set<std::pair<int, int>> *current_set = &white_positions;
    if (current_color == Color::BLACK) {
        current_set = &black_positions;
    }
    std::vector<std::pair<int, int>> kill_moves = {{2,  2},
                                                   {2,  -2},
                                                   {-2, 2},
                                                   {-2, -2}};
    std::vector<std::pair<int, int>> usual_moves = {{1,  1},
                                                    {1,  -1},
                                                    {-1, 1},
                                                    {-1, -1}};
    for (const auto &checker : *current_set) {
        int x = checker.first;
        int y = checker.second;
        for (const auto &move : usual_moves) {
            int new_x = x + move.first;
            int new_y = y + move.second;
            if (!CheckMove(x, y, new_x, new_y)) {
                continue;
            }
            for (const auto &enemy : usual_moves) {
                int enemy_x = new_x + enemy.first;
                int enemy_y = new_y + enemy.second;
                if (enemy_x < 0 || enemy_x >= FIELD_SIZE || enemy_y < 0 || enemy_y >= FIELD_SIZE) {
                    continue;
                }
                if (field[enemy_x][enemy_y] && field[enemy_x][enemy_y]->color != current_color) {
                    return false; // нашли врага, который нас съест на новом месте
                }
            }
            // нас после этого хода напрямую не убьют, проверим соседей:
            for (const auto &enemy : kill_moves) {
                int enemy_x = x + enemy.first;
                int enemy_y = y + enemy.second;
                if (enemy_x < 0 || enemy_x >= FIELD_SIZE || enemy_y < 0 || enemy_y >= FIELD_SIZE) {
                    continue;
                }
                if (field[enemy_x][enemy_y] && field[enemy_x][enemy_y]->color != current_color) {
                    int problem_x = (x + enemy_x) / 2;
                    int problem_y = (y + enemy_y) / 2;
                    if (field[problem_x][problem_y] &&
                        field[problem_x][problem_y]->color == current_color) {
                        return false; // нашли нашу шашку, которую побьют после хода
                    }
                }
            }


        }
    }
    return false;
}

bool Field::CheckStep() {
    std::set<std::pair<int, int>> *current_set = &white_positions;
    if (current_color == Color::BLACK) {
        current_set = &black_positions;
    }
    std::vector<std::pair<int, int>> usual_moves = {{1,  1},
                                                    {1,  -1},
                                                    {-1, 1},
                                                    {-1, -1}};
    for (const auto &checker : *current_set) {
        int x = checker.first;
        int y = checker.second;
        for (const auto &move : usual_moves) {
            int new_x = x + move.first;
            int new_y = y + move.second;
            if (!CheckMove(x, y, new_x, new_y)) {
                continue;
            }
            MoveUnit(x, y, new_x, new_y);
            return true;
        }
    }
    return false;
}

void Field::SetColor(Color color) {
    current_color = color;
}

void Field::InvertColor() {
    if (current_color == Color::BLACK) {
        current_color = Color::WHITE;
    } else {
        current_color = Color::BLACK;
    }
}


