#include "../include/play.h"

Play::Play(std::string color, int level): level(level) {
    if (color == "white") {
        first_color = Color::WHITE;
        field.SetColor(first_color);
    }
    if (color == "black") {
        first_color = Color::BLACK;
        field.SetColor(first_color);
    }
}


bool Play::StepBot() {

    if (field.CheckStepKill()) {
        field.InvertColor();
        return true;
    }

    if (level == 1) {
        field.InvertColor();
        return field.CheckSafeMove();
    }

    if (level == 2) {
        field.InvertColor();
        return field.CheckStep();
    }

}

void Play::StepPlayer() {
    // добавить обработку ошибок

    // первое число ввода - это количество шашек убитых на этом ходу игроком
    int count_kill;

    // изначальная позиция шашки
    int curr_x, curr_y;
    std::cin >> curr_x >> curr_y;

    if (count_kill == 0) {
        int next_x, next_y;
        std::cin >> next_x >> next_y;
        field.MoveUnit(curr_x, curr_y, next_x, next_y);
        return;
    }
    int prev_x = curr_x;
    int prev_y = curr_y;
    for (int i = 0; i < count_kill; ++i) {
        int next_x, next_y;
        std::cin >> next_x >> next_y;

        field.KillUnit((prev_x + next_x) / 2, (prev_y + next_y) / 2);

        std::swap(prev_x, next_x);
        std::swap(prev_y, next_y);
    }
    field.MoveUnit(curr_x, curr_y, prev_x, prev_y);

    field.InvertColor();
}


Color Play::Run() {

    if (first_color = Color::WHITE) {
        StepBot();
    }

    while(true) {

        StepPlayer();

        if (!StepBot()) {
            return Color::BLACK;
        }
    }
}