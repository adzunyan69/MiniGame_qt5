#include "game.h"

Game::Game() {
    // Расставим вручную как попало цветные блоки, можно добавить
    // какую-нибудь случайную генерацию
    field[0][0] = COLOR_RED;
    field[3][2] = COLOR_RED;
    field[0][4] = COLOR_RED;
    field[1][4] = COLOR_RED;
    field[3][0] = COLOR_RED;

    field[1][0] = COLOR_GREEN;
    field[4][0] = COLOR_GREEN;
    field[4][2] = COLOR_GREEN;
    field[2][2] = COLOR_GREEN;
    field[2][4] = COLOR_GREEN;

    field[2][0] = COLOR_BLUE;
    field[0][2] = COLOR_BLUE;
    field[1][2] = COLOR_BLUE;
    field[3][4] = COLOR_BLUE;
    field[4][4] = COLOR_BLUE;
    // Пусть блоки будут на 1,3,5-х строках в 2 и 4 столбцах
    // Для простоты проверки можно задать все блоки свободными
    for(int i = 0; i < fieldSize; i += 2)
        for(int j = 1; j < fieldSize; j += 2)
            //field[i][j] = FREE;
            field[i][j] = BLOCK;

    // Пусть свободные блоки будут соответственно
    // на 2, 4-х строках и в 2, 4-х столбцах
    for(int i = 1; i < fieldSize; i += 2)
        for(int j = 1; j < fieldSize; j += 2)
            field[i][j] = FREE;
}

QString Game::consolePrint() {
   QString str;
    for(unsigned i = 0; i < fieldSize; ++i) {
        for(unsigned j = 0; j < fieldSize; ++j)
            str += QString(convertEnumDebug(field[i][j])) + " ";
        str += "\n";
    }

    return str;
}

QChar Game::convertEnumDebug(ContentType ct) {
    switch(ct) {
    case BLOCK:
        return 'x';
    case FREE:
        return 'o';
    case COLOR_RED:
        return 'R';
    case COLOR_GREEN:
        return 'G';
    case COLOR_BLUE:
        return 'B';
    default:
        return ' ';
    }
}

pair<int, int> Game::move(int x, int y, MoveType moveType) {
    // Если как-то вышли за диапазон значений, то выходим отсюда
    if(x < 0 || x >= fieldSize || y < 0 || y >= fieldSize)
        // можно выкинуть исключение
        return pair<int, int>(-1, 0);

    // Незачем передвигать свободную или заблокированную клетку
    if(field[y][x] == ContentType::FREE || field[y][x] == ContentType::BLOCK)
        return pair<int, int>(-1, 0);

    switch(moveType) {
    case UP:
        if(y > 0 && field[y-1][x] == ContentType::FREE)
            swap(field[y][x], field[y-1][x]);
               return pair<int, int>(y-1, x);
    case DOWN:
        if((y < fieldSize-1) && field[y+1][x] == ContentType::FREE)
            swap(field[y][x], field[y+1][x]);
               return pair<int, int>(y+1, x);
    case LEFT:
        if(x > 0 && field[y][x-1] == ContentType::FREE)
            swap(field[y][x], field[y][x-1]);
               return pair<int, int>(y, x-1);
    case RIGHT:
        if(x < fieldSize-1 && field[y][x+1] == ContentType::FREE)
            swap(field[y][x], field[y][x+1]);
               return pair<int, int>(y, x+1);
    default:
        return pair<int, int>(-1, 0);
    }

}

bool Game::isWin() const {

    // Пусть первый столбце должен быть красным, второй зеленым, третий - синим
    for(int currentColor = ContentType::COLOR_RED; currentColor < fieldSize; currentColor += 2)
        for(int i = 0; i < fieldSize ; ++i)
            if(field[i][currentColor] != currentColor)
                return false;
    return true;
}
