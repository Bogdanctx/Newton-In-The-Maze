#include <iostream>
#include <vector>

#include "rlutil.h"

class Maze {
public:
    Maze(const int &dim = 25) : m_maze(std::vector<std::vector<char>>(dim + 1, std::vector<char>(dim + 1, '#'))) {};

    friend std::ostream &operator<<(std::ostream &os, Maze &maze);

    void generate();

private:
    std::vector<std::vector<char>> m_maze;
};

std::ostream &operator<<(std::ostream &os, Maze &maze) {
    for (const auto &i: maze.m_maze) {
        for (const auto &j: i) {
            os << j;
        }
        os << '\n';
    }
    return os;
}

void Maze::generate() {

}

class Player {
public:
    Player(const int &posX, const int &posY) : m_posX(posX), m_posY(posY) {};

    void set_newPosition(const int &key_pressed, const int &maze_dim);

    friend std::ostream &operator<<(std::ostream &os, Player &player);

private:
    int m_posX, m_posY;
};

std::ostream &operator<<(std::ostream &os, Player &player) {
    rlutil::setColor(rlutil::YELLOW);

    gotoxy(player.m_posX, player.m_posY);
    os << 'A';
    os.flush();
    rlutil::setColor(rlutil::WHITE);

    return os;
}

void Player::set_newPosition(const int &key_pressed, const int &maze_dim) {
    if (key_pressed == 119 && m_posY - 1 >= 0) { // w
        --m_posY;
    } else if (key_pressed == 97 && m_posX - 1 >= 0) { // a
        --m_posX;
    } else if (key_pressed == 115 && m_posY + 1 <= maze_dim) { // s
        ++m_posY;
    } else if (key_pressed == 100 && m_posX + 1 <= maze_dim) { // d
        ++m_posX;
    }
}

class Game {
public:
    Game(const int &maze_size) : m_maze(maze_size) {};

    void run();

private:
    Maze m_maze;
    Player m_player{5, 5};

    void handleEvent();

    void render();

    const int keys[4] = {119, 97, 115, 100}; // W, A, S, D
    bool m_isRunning = true;
};

void Game::run() {
    m_maze.generate();

    while (m_isRunning) {
        render();
        handleEvent();

        rlutil::msleep(300);
    }
}

void Game::render() {
    rlutil::cls();

    std::cout << m_maze;
    std::cout << m_player;
}

void Game::handleEvent() {
    const int key_pressed = rlutil::getkey();

    m_player.set_newPosition(key_pressed, 25);

    //std::cout<<(char)key<<' '<<key<<'\n';
}

int main() {
    int maze_dim = 25;
    std::cout << "Enter maze dimension (default = 25): ";
    std::cin >> maze_dim;

    Game game{maze_dim};

    game.run();

    return 0;
}