#include "minigame.h"
#include "ui_minigame.h"
#include "game.h"

MiniGame::MiniGame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MiniGame)
{
    ui->setupUi(this);

    game = new Game();
    field_size = game->getSize();

    // Сдвиг для кнопок, чтобы начиналис не от края
    int offset_x = 30, offset_y = 60;

    // Выводим иконки-подскази, показывающие, в каком порядке
    // собирать столбики
    for(size_t i = 0; i < field_size/2+1; ++i) {
        help_buttons.push_back(new QPushButton("", this));
        help_buttons.at(i)->move(i * 60 + offset_x, offset_y - 35);
        help_buttons.at(i)->resize(30, 30);
    }
    help_buttons.at(0)->setStyleSheet("background-color: rgb(255,70,70);");
    help_buttons.at(1)->setStyleSheet("background-color: rgb(70,255,70);");
    help_buttons.at(2)->setStyleSheet("background-color: rgb(70,70,255);");
    // Настраиваем все наши кнопки-клетки и привязываем сигнал-слоты
    for(size_t i = 0; i < field_size*field_size; ++i) {
        buttons.push_back(new QPushButton("", this));
        buttons.at(i)->move((i/field_size)*30 + offset_x, (i%field_size*30) + offset_y);
        buttons.at(i)->setObjectName(QString::number(i));
        buttons.at(i)->resize(30, 30);
        buttons.at(i)->setText(QString::number(i));
        QObject::connect(buttons.at(i), SIGNAL(clicked()), this, SLOT(field_buttons_clicked()));

    }

    // Делаем первую отрисовку
    render_field();

}

MiniGame::~MiniGame()
{
    delete ui;
}

void MiniGame::render_field()  {


    // Изменение цвета в зависимости от типа клетки
    for(size_t i = 0; i < buttons.size(); ++i) {
        QString color;

        auto button_type = game->getType(i/field_size, i%field_size);
        switch(button_type) {
        case Game::BLOCK:
            color = "background-color: rgb(180,180,180);";
            break;
        case Game::FREE:
            color = "background-color: rgb(255,255,255);";
            break;
        case Game::COLOR_RED:
            color = "background-color: rgb(255,70,70);";
            break;
        case Game::COLOR_GREEN:
            color = "background-color: rgb(70,255,70);";
            break;
        case Game::COLOR_BLUE:
            color = "background-color: rgb(70,70,255);";
            break;
        }

        // Если клетка выделена, то обведем в черную рамку
        if(index_button_clicked == i)
            color += "border: 4px solid rgb(0, 0, 0);";
        buttons.at(i)->setStyleSheet(color);
    }


    QString textBox= QString("debug_text\nВывод состояния поля для проверки:\n") + \
game->consolePrint() + \
QString("Управление: клик - выделение клетки, WASD - движения\n\
Для победы - собрать красный, зеленый и синий 1-й, 3-й и 5-й столбцы соответственно\n\
Текущая выделенная клетка: ") + QString::number(index_button_clicked) + \
"\nx: " + QString::number(index_button_clicked/field_size) +\
" y: " + QString::number(index_button_clicked%field_size) +\
"\nisWin: " + (game->isWin() ? "true" : "false");
    ui->textBrowser->setText(textBox);
}

void MiniGame::field_buttons_clicked() {
    // Получаем имя кнопки (задано ее порядковым номером)
    QString s = sender()->objectName();
    // Преобразовываем в число
    auto check_index_button = s.toInt();
    // Смотрим, какого типа выбранная клетка
    auto button_type = game->getType(check_index_button/field_size, check_index_button%field_size);
    // Выделяем ее, если это цветная клетка
    if(button_type != Game::ContentType::FREE && button_type != Game::ContentType::BLOCK)
        index_button_clicked = check_index_button;
    // Обновляем поле
    render_field();
}

// Важно: необходимо следить за раскладкой
// Нажатия будут восприниматься только на ангилйской раскладке
// По какой-то причине в Qt нажатия стрелок перехиватывают другие виджеты
// А решение выглядит очень костыльным для небольшой задачи,
// Поэтому управление переведно на WASD
void MiniGame::keyPressEvent(QKeyEvent *event) {
    // Если у нас выделена клетка, то смотрим на событие
    // и делаем движения в зависимости от него
    if(index_button_clicked > -1) {
        Game::MoveType moveType = Game::MoveType::NO_MOVE;
        switch(event->key()) {
        case Qt::Key_D:
            ui->textBrowser->setText("right");
            moveType = Game::MoveType::RIGHT;
            break;
        case Qt::Key_A:
            moveType = Game::MoveType::LEFT;
            break;
        case Qt::Key_W:
            moveType = Game::MoveType::UP;
            break;
        case Qt::Key_S:
            moveType = Game::MoveType::DOWN;
            break;
        }

        // Если надо сделать какое-то перемещение
        if(moveType != Game::MoveType::NO_MOVE) {
            // То перемещаем клетку и получаем новые координаты
            auto new_field = game->move(index_button_clicked/field_size, index_button_clicked%field_size, moveType);
            // Обозначаем новую выделенную клетку
            index_button_clicked = new_field.first + new_field.second * field_size;

            // TODO: исправить баг. при котором выделение выезжает на пустую клетку следующего/
            // прошлого столбца при движении вниз/вверх соответственно + убрать выделение с блоков
            if(new_field.first >= field_size || new_field.first < 0)
                index_button_clicked = -1;

            // Перерисовываем цвета клеток
            render_field();

            // Проверяем, победили ли
            if(game->isWin())
                ui->textBrowser->setText("the end");
        }

    }

}

