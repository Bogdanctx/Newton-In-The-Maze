#include <iostream>
#include <vector>
#include <chrono>
#include <random.hpp>

#include "rlutil.h"

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

    bool isPositionAvailable(const int &row, const int &col); // verifica daca o noua pozitie este buna
    void generate(); // genereaza un labirint
    void createHole(const int &row, const int &col); // distruge cei 8 vecini din jurul jucatorului cand a folosit bomba

    std::vector<std::pair<int, int>> getFreeCells(); // cauta toate locurile libere din labirint

private:
    [[nodiscard]] bool isInside(const int &row, const int &col) const; // verifica daca coordonatele sunt in interiorul labirintului
    std::vector<std::vector<char>> m_maze; // labirintul
    const int m_dim; // dimensiunea labirintului
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

void Maze::createHole(const int &row, const int &col) { // cand a fost bomba folosita se distrug cei 8 vecini din jurul jucatorului
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

std::vector<std::pair<int, int>> Maze::getFreeCells() { // cauta toate locurile libere din labirint
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

void Maze::generate() { // Pentru generarea labirintului a fost folosit algoritmul Sidewinder
    for (int col = 0; col < m_dim; col++) {
        m_maze[0][col] = ' ';
    }
    for (int row = 2; row < m_dim; row += 2) {
        std::vector<std::pair<int, int>> run;
        for (int col = 1; col < m_dim; col++) {
            m_maze[row][col] = ' ';
            run.emplace_back(row, col);

            bool carve_east = effolkronium::random_static::get(0, 100) > 30; // daca continui sa sap la dreapta

            if (carve_east && col + 1 < m_dim) {
                m_maze[row][col + 1] = ' ';
            }
            else { // sap in sus
                std::pair<int, int> randomCell = run[effolkronium::random_static::get(0, (int)run.size()-1)];
                m_maze[randomCell.first - 1][randomCell.second] = ' ';
                if (randomCell.first + 1 < m_dim) {
                    m_maze[randomCell.first + 1][randomCell.second] = ' ';
                }
                run.erase(run.begin(), run.end());
                ++col;
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
    int m_crtRow, m_crtCol; // pozitia curenta a jucatorului
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
    explicit Game(const int &maze_size) : m_maze(maze_size), bomb{0, effolkronium::random_static::get(2, maze_size - 1), rlutil::CYAN},
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
    bool m_isRunning; // flag pentru a mentine rularea jocului
    bool m_toggleRender; // flag ca sa fac render doar atunci cand s-a intamplat ceva
};

void Game::run() {
    m_maze.generate();

    const std::vector<std::pair<int, int>> mazeFreeCells = m_maze.getFreeCells();
    const int numberOfRandomObjects = effolkronium::random_static::get(3, 10);

    for (int i = 0; i < numberOfRandomObjects; i++) { // aici generez obiecte (mere) random in labirint
        const int randIndex = effolkronium::random_static::get(0, (int) mazeFreeCells.size() - 1);
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

        if (bomb.getPosition().second == m_mazeSize - 1) { // atunci cand o bomba a fost luata noua pozitie va fi in dreapta
            // conditia de sus verifica daca bomba a fost luata in functie de pozitia bombei

            const std::chrono::system_clock::time_point elapsedTime = std::chrono::system_clock::now();
            const int duration = (int) std::chrono::duration_cast<std::chrono::seconds>(elapsedTime - clock).count();

            if (duration >= 5) { // daca au trecut 5 secunde de cand bomba a fost luata atunci generez una noua
                const Object newBomb = Object{0, effolkronium::random_static::get(2, m_mazeSize - 2), rlutil::CYAN};
                bomb = newBomb;
                m_toggleRender = true;
            }
        }

    }

    rlutil::cls();

    if(objects.empty()) {
        std::cout << "Newton find its apples. He may now study the laws of gravity.\n";
    }
    else {
        std::cout << "Newton couldn't find its apples.\n";
    }
}

void Game::render() {
    rlutil::cls();

    std::cout << m_maze;

    // aici marchez iesirea din labirint
    const int objectsRemained = (int) objects.size();
    gotoxy(1, 1);
    if (objectsRemained != 0) {
        rlutil::setColor(rlutil::LIGHTRED);
        std::cout << ">>";
    } else {
        gotoxy(1, 1);
        rlutil::setColor(rlutil::LIGHTGREEN);
        std::cout << "<<";
    }
    rlutil::setColor(rlutil::WHITE);

    std::cout << bomb;
    for (Object &obj: objects) { // afisez merele pe ecran
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

    switch (std::tolower(key_pressed)) {
        case rlutil::KEY_SPACE: {
            objects.erase(
            std::remove_if(objects.begin(), objects.end(), [&](Object item) {
                    const std::pair<int, int> objPosition = item.getPosition();
                    return playerPosition == objPosition;
            }), objects.end()); // daca jucatorul se afla pe un obiect inseamna ca a luat obiectul => il sterg din vector

            std::pair<int, int> bombPosition = bomb.getPosition();

            if (playerPosition == bombPosition && bombPosition.second != m_mazeSize - 1) { // daca jucatorul a luat bomba
                // si bomba a fost generata
                m_player.setHasBomb(true);
                const Object offBomb{0, m_mazeSize - 1, rlutil::LIGHTGREEN};
                bomb = offBomb;

                clock = std::chrono::system_clock::now(); // cronometrez cat timp a trecut de cand bomba a fost luata
            }

            renderFlag = true;

            break;
        }

        case 102: { // f
            if (m_player.getHasBomb()) { // jucatorul vrea sa foloseasca bomba
                m_maze.createHole(crtRow, crtCol);
                m_player.setHasBomb(false);
            }

            renderFlag = true;
            break;
        }
        case 119: { // w
            if (m_maze.isPositionAvailable(crtRow - 1, crtCol)) {
                m_player = std::make_pair(crtRow - 1, crtCol);
            }

            renderFlag = true;
            break;
        }
        case 97: { // a
            if (m_maze.isPositionAvailable(crtRow, crtCol - 1)) {
                m_player = std::make_pair(crtRow, crtCol - 1);
            }

            renderFlag = true;
            break;
        }
        case 115: { // s
            if (m_maze.isPositionAvailable(crtRow + 1, crtCol)) {
                m_player = std::make_pair(crtRow + 1, crtCol);
            }

            renderFlag = true;
            break;
        }
        case 100: { // d
            if (m_maze.isPositionAvailable(crtRow, crtCol + 1)) {
                m_player = std::make_pair(crtRow, crtCol + 1);
            }

            renderFlag = true;
            break;
        }
        case 113: { // q - exit
            m_isRunning = false;
            break;
        }
        default:
            break;
    }
}

//////////////////////////// END GAME CLASS ////////////////////////////////////////


int main() {
    rlutil::setCursorVisibility(false);

    int dim_maze = 25;

    std::cout << "Enter maze size (default=25, min=10, max=30): ";
    std::cin >> dim_maze;

    if (dim_maze < 10) {
        dim_maze = 10;
    }
    if (dim_maze > 30) {
        dim_maze = 30;
    }

    Game game{dim_maze};
    game.run();

    return 0;
}