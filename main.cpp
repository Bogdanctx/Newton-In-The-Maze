#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <climits>

#include "rlutil.h"

inline int random(const int max_val = INT_MAX, const int min_val = 0) {
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    return static_cast<int>(rng() % (max_val - min_val) + min_val);
}

////////////////////// OBJECT CLASS DEFINITION ////////////////////////////

class Object {
public:
    Object(const int &row, const int &col, const int &color = rlutil::LIGHTRED) : m_crtRow(row), m_crtCol(col),
                                                                                  m_color(color) {}

    ~Object() = default;

    friend std::ostream &operator<<(std::ostream &out, const Object &object);

    std::pair<int, int> getPosition();

private:
    int m_crtRow, m_crtCol, m_color;
};

std::ostream &operator<<(std::ostream &out, const Object &object) {
    rlutil::setColor(object.m_color);
    gotoxy(object.m_crtCol + 1, object.m_crtRow + 1);
    out << 'O';
    rlutil::setColor(rlutil::WHITE);
    return out;
}

std::pair<int, int> Object::getPosition() {
    return std::make_pair(m_crtRow, m_crtCol);
}

////////////////////////////// END OBJECT CLASS ///////////////////////////////////////











////////////////////// MAZE CLASS DEFINITION ////////////////////////////

class Maze {
public:
    explicit Maze(const int &dim) : m_maze(std::vector<std::vector<char>>(dim, std::vector<char>(dim, '#'))),
                                    m_dim(dim) {};
    ~Maze() = default;

    friend std::ostream &operator<<(std::ostream &out, const Maze &maze);

    bool isPositionAvailable(const int &row, const int &col);
    void generate();
    void createHole(const int &row, const int &col);

    std::vector<std::pair<int, int>> getFreeCells();

private:
    [[nodiscard]] bool isInside(const int &row, const int &col) const;
    std::vector<std::vector<char>> m_maze;
    const int m_dim;
};

std::ostream &operator<<(std::ostream &out, const Maze &maze) {
    for (const auto &i: maze.m_maze) {
        for (const auto &j: i) {
            out << j;
        }
        out << '\n';
    }
    return out;
}

bool Maze::isInside(const int &row, const int &col) const {
    return row >= 0 && col >= 0 && row < m_dim && col < m_dim;
}

void Maze::createHole(const int &row, const int &col) {
    const int drdc[8][2] = {{-1, -1},
                            {-1, 0},
                            {-1, 1},
                            {0,  1},
                            {1,  1},
                            {1,  0},
                            {1,  -1},
                            {0,  -1}};

    for (const auto &d: drdc) { // cppcheck-suppress constVariable
        const int newRow = row + d[0];
        const int newCol = col + d[1];
        if (isInside(newRow, newCol) && m_maze[newRow][newCol] == '#') {
            m_maze[newRow][newCol] = ' ';
        }
    }
}

std::vector<std::pair<int, int>> Maze::getFreeCells() {
    std::vector<std::pair<int, int>> result;
    for (int row = 1; row < m_dim; row++) {
        for (int col = 0; col < m_dim; col++)
            if (m_maze[row][col] == ' ') {
                result.emplace_back(row, col);
            }
    }
    return result;
}

bool Maze::isPositionAvailable(const int &row, const int &col) {
    return isInside(row, col) && m_maze[row][col] != '#';
}

void Maze::generate() {
    for (int col = 0; col < m_dim; col++) {
        m_maze[0][col] = ' ';
    }
    for (int row = 2; row < m_dim; row += 2) {
        std::vector<std::pair<int, int>> run;
        for (int col = 1; col < m_dim; col++) {
            m_maze[row][col] = ' ';
            run.emplace_back(row, col);

            bool carve_east = random(101) > 30;

            if (carve_east && col + 1 < m_dim) {
                m_maze[row][col + 1] = ' ';
            } else {
                std::pair<int, int> randomCell = run[random(static_cast<int>(run.size()))];
                m_maze[randomCell.first - 1][randomCell.second] = ' ';
                if (randomCell.first + 1 < m_dim) {
                    m_maze[randomCell.first + 1][randomCell.second] = ' ';
                }
                run.erase(run.begin(), run.end());
                col++;
            }
        }
    }
}

////////////////////////////// END MAZE CLASS ///////////////////////////////////////
















/////////////////////////// PLAYER CLASS DEFINITION ///////////////////////////////////////

class Player {
public:
    Player() : m_crtRow(0), m_crtCol(0), m_hasBomb(false) {};
    ~Player() = default;

    void setHasBomb(bool val);
    [[nodiscard]] bool getHasBomb() const;
    std::pair<int, int> getPosition();

    friend std::ostream &operator<<(std::ostream &out, const Player &player);
    Player& operator=(const std::pair<int, int> &pos);

private:
    int m_crtRow, m_crtCol;
    bool m_hasBomb;
};

Player &Player::operator=(const std::pair<int, int> &pos) {
    m_crtRow = pos.first;
    m_crtCol = pos.second;
    return *this;
}

std::ostream& operator<<(std::ostream &out, const Player &player) {
    rlutil::setColor(rlutil::BROWN);

    gotoxy(player.m_crtCol + 1, player.m_crtRow + 1);
    out << 'T';
    rlutil::setColor(rlutil::WHITE);

    return out;
}

void Player::setHasBomb(const bool val) {
    m_hasBomb = val;
}

bool Player::getHasBomb() const {
    return m_hasBomb;
}

std::pair<int, int> Player::getPosition() {
    return std::pair<int, int>{m_crtRow, m_crtCol};
}

/////////////////////////////////// END PLAYER CLASS /////////////////////////////////////////////











//////////////////////// GAME CLASS DEFINITION //////////////////////////////////////////

class Game {
public:
    explicit Game(const int &maze_size) : m_maze(maze_size), bomb{0, random(maze_size - 1, 2), rlutil::CYAN},
                                          m_mazeSize(maze_size), m_isRunning(true), m_toggleRender(true) {};
    ~Game() = default;

    void run();

private:
    void handleEvent(bool &renderFlag);
    void render();

    Maze m_maze;
    Player m_player;
    Object bomb;
    std::vector<Object> objects;
    std::chrono::system_clock::time_point clock;

    const int m_mazeSize;
    bool m_isRunning;
    bool m_toggleRender;
};

void Game::run() {
    rlutil::setBackgroundColor(rlutil::DARKGREY);
    m_maze.generate();
    const std::vector<std::pair<int, int>> mazeFreeCells = m_maze.getFreeCells();
    const int numberOfRandomObjects = random(10, 3) + 1;

    for (int i = 0; i < numberOfRandomObjects; i++) {
        const int randIndex = random(static_cast<int>(mazeFreeCells.size()), 2);
        const std::pair<int, int> randomPos = mazeFreeCells[randIndex];
        const Object obj{randomPos.first, randomPos.second};
        objects.push_back(obj);
    }

    while (m_isRunning) {
        handleEvent(m_toggleRender);

        if (m_toggleRender) {
            render();
            m_toggleRender = false;
        }

        if (bomb.getPosition().second == m_mazeSize - 1) {
            const std::chrono::system_clock::time_point elapsedTime = std::chrono::system_clock::now();
            const int duration = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(
                    elapsedTime - clock).count());

            if (duration >= 5) {
                const Object newBomb = Object{0, random(m_mazeSize - 3) + 2, rlutil::CYAN};
                bomb = newBomb;
                m_toggleRender = true;
            }
        }

    }

    rlutil::setBackgroundColor(rlutil::BLACK);
}

void Game::render() {
    rlutil::cls();

    std::cout << m_maze;

    const int objectsRemained = static_cast<int>(objects.size());
    if (objectsRemained != 0) {
        gotoxy(1, 1);
        rlutil::setColor(rlutil::LIGHTRED);
        std::cout << ">>";
        rlutil::setColor(rlutil::WHITE);
    } else {
        gotoxy(1, 1);
        rlutil::setColor(rlutil::LIGHTGREEN);
        std::cout << "<<";
        rlutil::setColor(rlutil::WHITE);
    }

    std::cout << bomb;
    for (Object &obj: objects) {
        std::cout << obj;
    }

    std::cout << m_player;
    std::cout.flush();
}

void Game::handleEvent(bool &renderFlag) {
    const int key_pressed = rlutil::nb_getch();
    std::pair<int, int> playerPosition = m_player.getPosition();
    const int crtRow = playerPosition.first;
    const int crtCol = playerPosition.second;

    if (objects.empty() && crtRow == 0 && crtCol == 0) {
        m_isRunning = false;
        return;
    }

    switch (key_pressed) {
        case rlutil::KEY_SPACE: {
            std::erase_if(objects, [&](Object item) {
                const std::pair<int, int> objPosition = item.getPosition();
                return playerPosition == objPosition;
            });

            std::pair<int, int> bombPosition = bomb.getPosition();

            if (playerPosition == bombPosition && bombPosition.second != m_mazeSize - 1) {
                m_player.setHasBomb(true);
                const Object offBomb{0, m_mazeSize - 1, rlutil::LIGHTGREEN};
                bomb = offBomb;

                clock = std::chrono::system_clock::now();
            }

            renderFlag = true;

            break;
        }

        case 102: // f
        case 70: {// F
            if (m_player.getHasBomb()) {
                m_maze.createHole(crtRow, crtCol);
                m_player.setHasBomb(false);
            }

            renderFlag = true;
            break;
        }
        case 119: // w
        case 87: { // W
            if (m_maze.isPositionAvailable(crtRow - 1, crtCol)) {
                m_player = std::make_pair(crtRow - 1, crtCol);
            }

            renderFlag = true;
            break;
        }
        case 97: // a
        case 65: { // A
            if (m_maze.isPositionAvailable(crtRow, crtCol - 1)) {
                m_player = std::make_pair(crtRow, crtCol - 1);
            }

            renderFlag = true;
            break;
        }
        case 115: // s
        case 83: { // S
            if (m_maze.isPositionAvailable(crtRow + 1, crtCol)) {
                m_player = std::make_pair(crtRow + 1, crtCol);
            }

            renderFlag = true;
            break;
        }
        case 100: // d
        case 68: { // D
            if (m_maze.isPositionAvailable(crtRow, crtCol + 1)) {
                m_player = std::make_pair(crtRow, crtCol + 1);
            }

            renderFlag = true;
            break;
        }
        default:
            break;
    }
}

//////////////////////////// END GAME CLASS ////////////////////////////////////////


int main() {
    rlutil::setCursorVisibility(false);

    int dim_maze;

    std::cout << "Enter maze size (default=25, min=10, max=35): ";
    std::cin >> dim_maze;

    if (dim_maze < 10) {
        dim_maze = 10;
    }
    if (dim_maze > 35) {
        dim_maze = 35;
    }

    Game game{dim_maze};
    game.run();

    rlutil::cls();
    rlutil::setColor(rlutil::LIGHTGREEN);
    std::cout << "Newton found all the apples. He may now study the laws of gravity\n";

    system("pause");

    return 0;
}