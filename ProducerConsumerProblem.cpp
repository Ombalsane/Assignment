#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

using namespace std;

const int BUFFER_SIZE = 5;

queue<int> buffer;
mutex mtx;
condition_variable not_full, not_empty;

// Producer function
void producer() {
    int item = 1;
    while (true) {
        unique_lock<mutex> lock(mtx);
        not_full.wait(lock, [] { return buffer.size() < BUFFER_SIZE; });

        // Add the item to the buffer
        buffer.push(item);
        cout << "Produced: " << item << endl;
        item++;

        not_empty.notify_one();
    }
}

// Consumer function
void consumer() {
    while (true) {
        unique_lock<mutex> lock(mtx);
        not_empty.wait(lock, [] { return !buffer.empty(); });

        int item = buffer.front();
        buffer.pop();
        cout << "Consumed: " << item << endl;

        not_full.notify_one();
    }
}

int main() {
    thread producer_thread(producer);
    thread consumer_thread(consumer);

    producer_thread.join();
    consumer_thread.join();

    return 0;
}
