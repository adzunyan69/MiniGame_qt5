#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include <QChar>
#include <QString>

using std::string;
using std::cout;
using std::endl;
using std::swap;
using std::pair;

class Game
{
public:
    Game();

    // Перечисление возможных типов блоков
    enum ContentType { COLOR_RED = 0, COLOR_GREEN = 2, COLOR_BLUE = 4, FREE, BLOCK  };
    // Перечесление возможных перемещений
    enum MoveType {UP, DOWN, LEFT, RIGHT, NO_MOVE };

    // Возвращает QString, содержащий поле
    // в текстовом формате для консоли
    QString consolePrint();

    // Движение клетки (x, y) в направлении moveType
    // Возвращает (x, y), в которую клетка переместилась,
    // либо (-1, 0), если идти некуда
    pair<int, int> move(int x, int y, MoveType moveType);

    // Возвращает тип блока, хранимый в клетке
    ContentType getType(const int x, const int y) const {
        return field[y][x];
    }

    // Возвращает ширину/длину поля
    int getSize() const {
        return fieldSize;
    }

    // Возвращает true, если столбцы совпадают с победными цветами,
    // false иначе
    bool isWin() const;
private:
    // Размер поля 5x5
    const static int fieldSize = 5;

    // Массив содержит все блоки и их типы
    ContentType field[fieldSize][fieldSize];

    // Вспомогательный метод для преобразования перечисления
    // в символы при выводе в консоль для проверки резульаттов (до графики)
    QChar convertEnumDebug(ContentType ct);

};


#endif // GAME_H
