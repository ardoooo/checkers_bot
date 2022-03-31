#pragma once

#include "field.h"
#include <iostream>


class Play {
public:

    //конструктор от цвета начинающего
    //значения color могут быть "white" или "black"
    Play(std::string color, int level);

    // возвращает цвет победителя
    // бот всегда белый, игрок всегда черный
    Color Run();

private:
    Field field;
    Color first_color;

    // уровень интеллекта бота, принимает значения 1 или 2
    int level;

    // делает ход ботом, если ход существует, возвращает true, иначе false
    bool StepBot();

    //принимает ход игрока
    void StepPlayer();
};