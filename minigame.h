#ifndef MINIGAME_H
#define MINIGAME_H

#include <QMainWindow>
#include <QString>
#include <QList>
#include <QPushButton>

class Game;

namespace Ui {
class MiniGame;
}

class MiniGame : public QMainWindow
{
    Q_OBJECT

public:
    explicit MiniGame(QWidget *parent = 0);

    // Сама игра
    Game *game;

    // Вектор всех кнопок, имитирующих клетки на поле
    QVector<QPushButton*> buttons;
    QVector<QPushButton*> help_buttons;

    // Размер поля, использумый при задании количества кнопок и индексации
    int field_size;

    // Текущая нажатая кнопка (клетка поля)
    int index_button_clicked = -1;

    // Отрисовка поля (обновление цветов в соответствии с перемещениями)
    void render_field();

    ~MiniGame();

private slots:
    void field_buttons_clicked();

private:
    Ui::MiniGame *ui;

protected:
    virtual void keyPressEvent(QKeyEvent *event);
};

#endif // MINIGAME_H
