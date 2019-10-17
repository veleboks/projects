#include <iostream>
#include <conio.h>
#include <string>
#include <cmath>
#include <windows.h>
#include <ctime>
#include <queue>

using namespace std;

enum STATE {START, GAMEOVER, PAUSE, QUIT};
enum eDirection {UP, STOP, DOWN};


class Point {
public:
    float x, y;
    static queue<int> x_points;
    static queue<int> y_points;
    void set_x(float x_) {
        x = x_;
    }
    void set_y(float y_) {
        y = y_;
    }
    int get_x() {
        return int(round(x));
    }
    int get_y() {
        return int(round(y));
    }
};

queue<int> Point::x_points;
queue<int> Point::y_points;

class Arcanoid: public Point {
private:
    eDirection dir;
    int width, height, length;
public:
    static int players_count;
    Arcanoid(int len, int w = 40, int h = 40) {
        width = w;
        height = h;
        length = len;
        if (Arcanoid :: players_count == 0)
            x = 5;
        else
            x = w - 6;
        Arcanoid :: players_count++;
    }
    void begin() {
        y = height / 2 - 1;
        dir = STOP;
    }
    void move(float speed) {
        if(dir == DOWN && y + length/2 < height - 1)
            y+=speed;
        else if (dir == UP && y - length/2 > 0)
            y-=speed;
        for (int i = -length/2; i <= length/2; ++i) {
            Point :: x_points.push(get_x());
            Point :: y_points.push(get_y() + i);
        }
    }
    void set_dir(int input) {
        if (input == 'w')
            dir = UP;
        else if (input == 's')
            dir = DOWN;
        else
            dir = STOP;
    }
    void AI(int ball_y) {
            if (y > ball_y)
                dir = UP;
            else if (y < ball_y)
                dir = DOWN;
            else
                dir = STOP;
    }
    void info() {
        cout << "\nArcanoid " << Arcanoid::players_count + 1 << ":";
        cout << "\n\tx(float): " << x;
        cout << "\n\ty(float): " << y;
        cout << "\n\tx: " << get_x();
        cout << "\n\ty: " << get_y();
        cout << "\n\twidth: " << width;
        cout << "\n\theight: " << height;
    }
};

int Arcanoid::players_count = 0;

class Ball: public Point {
private:
    int alfa;
    int width, height;
public:
    Ball(int w = 40, int h = 40) {
        width = w;
        height = h;
    }
    void begin(bool flg) {
        const int N = 6;
        x = width / 2 - 1;
        y = height / 2 - 1;
        int good_angles[N] = {45, 75, 40, 50, 65, 70};
        alfa = 180 * int(flg) - good_angles[rand() % N];
    }
    void set_alfa(int a) {
        alfa = a;
    }
    float get_alfa() {
        return alfa;
    }
    void move(int ay1, int ay2, int len) {
        if((int(round(y+sin(alfa * 3.14f / 180.0f))) >= height) || (int(round(y+sin(alfa * 3.14f / 180.0f))) < 0))
            alfa = -alfa % 360;
        if((int(round(x+cos(alfa * 3.14f / 180.0f))) >= width) || (int(round(x+cos(alfa * 3.14f / 180.0f))) < 0))
            alfa = (180 - alfa) % 360;
        for (int i = -len/2; i <= len/2; ++i)
            if (get_y() == ay1 + i)
                if(int(round(x+cos(alfa * 3.14f / 180.0f))) == 5)
                    alfa = (180 - alfa) % 360;

        for (int i = -len/2; i <= len/2; ++i)
            if (get_y() == ay2 + i)
                if(int(round(x+cos(alfa * 3.14f / 180.0f))) == width - 6)
                    alfa = (180 - alfa) % 360;

        x+=cos(alfa * 3.14f / 180.0f);
        y+=sin(alfa * 3.14f / 180.0f);

        Point :: x_points.push(get_x());
        Point :: y_points.push(get_y());
    }
    void info() {
        cout << "\nBall: ";
        cout << "\n\tx(float): " << x;
        cout << "\n\ty(float): " << y;
        cout << "\n\tx: " << get_x();
        cout << "\n\ty: " << get_y();
        cout << "\n\talfa: " << alfa;
        cout << "\n\twidth: " << width;
        cout << "\n\theight: " << height;

    }
};

class Game {
private:
    const string SCORE_NUM = " 00000000 "
                             "0000000000"
                             "00      00"
                             "00      00"
                             "00      00"
                             "00      00"
                             "00      00"
                             "00      00"
                             "0000000000"
                             " 00000000 "

                             "    00    "
                             "    00    "
                             "    00    "
                             "    00    "
                             "    00    "
                             "    00    "
                             "    00    "
                             "    00    "
                             "    00    "
                             "    00    "

                             "  00  00  "
                             "  00  00  "
                             "  00  00  "
                             "  00  00  "
                             "  00  00  "
                             "  00  00  "
                             "  00  00  "
                             "  00  00  "
                             "  00  00  "
                             "  00  00  "

                             "00  00  00"
                             "00  00  00"
                             "00  00  00"
                             "00  00  00"
                             "00  00  00"
                             "00  00  00"
                             "00  00  00"
                             "00  00  00"
                             "00  00  00"
                             "00  00  00"

                             "00 00   00"
                             "00 00   00"
                             "00 00   00"
                             "00 00   00"
                             "00 00   00"
                             "00 00   00"
                             "00 00   00"
                             "00 00   00"
                             "00  00 00 "
                             "00   000  "

                             "000    000"
                             "000    000"
                             "000    000"
                             "000    000"
                             "000    000"
                             "000    000"
                             "000    000"
                             "000    000"
                             " 000  000 "
                             "  000000  ";

    STATE state;
    char map[40][60];
    int width, height;
    int key;
    int score[2];
public:
    Game(int w = 40, int h = 40) {
        width = w;
        height = h;
        srand(time(NULL));
    }

    void begin() {
        state = START;
        score[0] = 0;
        score[1] = 0;
    }

    void set_map() {
        for (int i = 0; i < height; ++i)
            for (int j = 0; j < width; ++j)
                map[i][j] = ' ';
        while(!Point::x_points.empty() && !Point::y_points.empty()) {
            map[Point::y_points.front()][Point::x_points.front()] = '#';
            Point :: x_points.pop();
            Point :: y_points.pop();
        }
    }

    int check_goal(int ball_x) {
        if (ball_x > width - 6)
            return 0;
        else if (ball_x < 5)
            return 1;
        return 2;
    }

    void goal(int winner) {
        score[winner]++;
    }

    bool get_winner() {
            if (score[0] >= score[1])
                return true;
            return false;
    }

    void show_score() {
        string str = "\n";

        for (int i = 0; i < 100; i+=10) {
            str += " " + SCORE_NUM.substr(i + score[0]*100, 10) + "                    "
                                                                  "                  " + SCORE_NUM.substr(i + score[1]*100, 10) + "\n";
        }

        cout << str;

    }

    STATE get_state() {
        return state;
    }
    void set_state(STATE s) {
        state = s;
    }

    void input() {
        if (_kbhit())
            key = _getch();
        else
            key = 0;

        if (key == 'p') state = PAUSE;
        else if (key == 's' || key == 'w') state = START;
        else if (key == 'q') state = QUIT;
    }

    int get_key() {
        return key;
    }

    bool is_end() {
        return score[0] == 5 || score[1] == 5;
    }

    void show() {
        system("cls");
        string str = "";
        for (int i = 0; i < width; ++i)
            str += "#";
        str += "\n";
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j)
                str += map[i][j];
            str += "\n";
        }
        for (int i = 0; i < width; ++i)
            str += "#";
        str += "\n";
        cout << str;
    }
    void info() {
        cout << "\nEvent: ";
        cout << "\n\t width: " << width;
        cout << "\n\t height: " << height;
    }

};

int main()
{
    const int WIDTH = 60;
    const int HEIGHT = 40;
    const int ARC_LEN = 8;
    const float PLAYER_SPEED = 1;
    const float AI_SPEED = 0.4f;

    Arcanoid * player = new Arcanoid(ARC_LEN, WIDTH, HEIGHT);
    Arcanoid * machine = new Arcanoid(ARC_LEN, WIDTH, HEIGHT);
    Ball * ball = new Ball(WIDTH, HEIGHT);
    Game * e = new Game(WIDTH, HEIGHT);

    player->begin();
    machine->begin();
    ball->begin(true);
    e->begin();

    e->set_state(PAUSE);

    while(e->get_state() != QUIT) {
        e->input();
        player->set_dir(e->get_key());
        player->move(PLAYER_SPEED);
        machine->AI(ball->get_y());
        machine->move(AI_SPEED);
        ball->move(player->get_y(), machine->get_y(), ARC_LEN);
        if (e->check_goal(ball->get_x()) == 0 || e->check_goal(ball->get_x()) == 1) {
            e->goal(e->check_goal(ball->get_x()));
            e->set_state(PAUSE);
            player->begin();
            machine->begin();
            ball->begin(e->get_winner());
            if (e->is_end())
                e->set_state(QUIT);
        }
        e->set_map(); // сборка поля
        e->show(); // вывод поля
        e->show_score();
        ball->info();
        player->info();
        machine->info();
        e->info();
        if (e->get_state() == PAUSE)
            while(e->get_state() == PAUSE) e->input();
    }

    return 0;
}
