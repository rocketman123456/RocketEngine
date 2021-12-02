#include <iostream>
#include <thread>
#include <mutex>
#include "EventSystem/Event.h"

using namespace std;
using namespace Rocket;

mutex cout_lock;
#define trace(x) { scoped_lock<mutex> lock(cout_lock); cout << x << endl; }

const int COUNT = 3;

void manual() {
    ManualEvent e;
    
    for(int i = 0; i < COUNT; ++i)
        thread([&](){
            trace("manual " << this_thread::get_id() << " blocked");
            e.wait();
            trace("manual " << this_thread::get_id() << " unblocked");
        }).detach();
    
    this_thread::sleep_for(500ms);
    e.signal();
    this_thread::sleep_for(500ms);
    e.reset();
    
    for(int i = 0; i < COUNT; ++i) {
        thread([&](){
            trace("manual " << this_thread::get_id() << " blocked");
            e.wait();
            trace("manual " << this_thread::get_id() << " unblocked");
        }).detach();
    }

    this_thread::sleep_for(500ms);
    e.signal();
    this_thread::sleep_for(500ms);
}

void automatic() {
    AutoEvent e;

    for(int i = 0; i < COUNT; ++i)
        thread([&](){
            trace("auto " << this_thread::get_id() << " blocked");
            e.wait();
            trace("auto " << this_thread::get_id() << " unblocked");
        }).detach();

    for(int i = 0; i < COUNT; ++i) {
        this_thread::sleep_for(500ms);
        e.signal();
    }

    this_thread::sleep_for(500ms);
}

int main(int argc, char** argv) {
    manual();
    automatic();
    return 0;
}
