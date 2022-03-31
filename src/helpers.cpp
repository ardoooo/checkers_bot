#include <vector>
#include <memory>
#include "../include/unit.h"

void KillDFS(std::vector<std::vector<bool>> &used, std::vector<std::vector<bool>> &eaten,
             const std::vector<std::vector<std::unique_ptr<Unit>>> &field,
             const Color current_color, int x, int y, const int field_size = 8) {
    used[x][y] = true;
    std::vector<std::pair<int, int>> kill_moves = {{2,  2},
                                                   {2,  -2},
                                                   {-2, 2},
                                                   {-2, -2}};
    for (const auto &move : kill_moves) {
        int new_x = x + move.first;
        int new_y = y + move.second;
        if (new_x < 0 || new_x >= field_size || new_y < 0 || new_y >= field_size) {
            continue;
        }
        if (used[new_x][new_y]) { // уже проверяли
            continue;
        }
        if (field[new_x][new_y]) { // стоит другая шашка
            continue;
        }
        if (!field[(x + new_x) / 2][(y + new_y) / 2]) { // нечего есть
            continue;
        }
        if (field[(x + new_x) / 2][(y + new_y) / 2]->color == current_color) { // едим своего
            continue;
        }
        // если все условия выполнены, то едим:
        eaten[(x + new_x) / 2][(y + new_y) / 2] = true;
        KillDFS(used, eaten, field, current_color, new_x, new_y, field_size);
    }
}

// реализация не убирает сразу же шашку, которую съели, но это и не требуется,
// так как мы на нее не сможем насткпить из-за четности
int TryPerformKill(const std::vector<std::vector<std::unique_ptr<Unit>>> &field, int x, int y,
                   const std::vector<std::pair<int, int>> &killing_order,
                   int &result_x, int &result_y, std::vector<std::pair<int, int>> &killed,
                   int field_size = 8
) {
    int count = 0;
    int x_current = x;
    int y_current = y;
    for (const auto &victim : killing_order) {
        int victim_x = victim.first;
        int victim_y = victim.second;
        int x_diff = victim_x - x_current;
        int y_diff = victim_y - y_current;
        if (x_diff * x_diff != 1 || y_diff * y_diff != 1) {
            break;
        }
        x_current += 2 * x_diff;
        y_current += 2 * y_diff;
        if (x_current < 0 || x_current >= field_size || y_current < 0 || y_current >= field_size) {
            break;
        }
        if (field[x_current][y_current]) {
            break;
        }
        ++count;
        result_x = x_current;
        result_y = y_current;
        killed.push_back(victim);
    }
    return count;
}
