#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <string>

using namespace std;

const int WIDTH = 100;
const int HEIGHT = 30;
const int SPEED = 1;
const int DELAY_MS = 100;

mutex printMutex;

class Vehicle {
public:
    string name;
    int x, y;
    int directionX;
    int directionY;

    Vehicle(string n, int x_pos, int y_pos, int dx, int dy) 
        : name(n), x(x_pos), y(y_pos), directionX(dx), directionY(dy) {}

    void moveX() {
        while (true) {
            {
                lock_guard<mutex> lock(printMutex);
                cout << "[Auto] " << name << " on X=" << x << endl;
            }
            
            x += directionX * SPEED;

            if (x >= WIDTH || x <= 0) {
                directionX *= -1;
            }
            this_thread::sleep_for(chrono::milliseconds(DELAY_MS));
        }
    }

    void moveY() {
        while (true) {
            {
                lock_guard<mutex> lock(printMutex);
                cout << "[Moto] " << name << " on Y=" << y << endl;
            }

            y += directionY * SPEED;

            if (y >= HEIGHT || y <= 0) {
                directionY *= -1;
            }
            this_thread::sleep_for(chrono::milliseconds(DELAY_MS));
        }
    }
};

int main() {
    vector<thread> threads; 

    threads.emplace_back([]() { 
        Vehicle car("Tesla", 0, 5, 1, 0); 
        car.moveX(); 
    });
    
    threads.emplace_back([]() { 
        Vehicle moto("Yamaha", 20, 0, 0, 1); 
        moto.moveY(); 
    });

    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }

    return 0;
}