#include <iostream>
#include <thread>
#include <queue>
#include <mutex>

std::mutex done_lock,queue_lock,count_lock;

int main() {
	size_t count = 0;
	bool done = false;
	std::queue<int> items;
	std::thread producer([&]() {
		for (int i = 0; i < 10000; ++i) {
			// ... some code may be here ...
			queue_lock.lock();
			items.push(i);
			queue_lock.unlock();
			count_lock.lock();
			count++;
			count_lock.unlock();
		}
		done_lock.lock();
		done = true;
		done_lock.unlock();
	});
	std::thread consumer([&]() {
		done_lock.lock();
		while (!done) {
			done_lock.unlock();
			while (!items.empty()) {
				queue_lock.lock();
				std::cout << items.front() << ' ';
				items.pop();
				queue_lock.unlock();
				// ...
				count_lock.lock();
				count--;
				count_lock.unlock();
			}
			std::cout << std::endl;
			done_lock.lock();
		}
		done_lock.unlock();
	});
	producer.join();
	consumer.join();
	std::cout << count << std::endl;
	return 0;
}

