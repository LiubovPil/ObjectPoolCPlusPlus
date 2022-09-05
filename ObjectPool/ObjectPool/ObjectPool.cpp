// Tasks from the course "Fundamentals of C++: Red Belt
// Done by Liubov Piliagina

#include "Test_runner.h"

#include <iostream>
#include <string>
#include <queue>
#include <set>
#include <stdexcept>

using namespace std;

template <class T>
class ObjectPool {
public:
	//template <typename T>
	//T* ObjectPool<T>::Allocate() {...}
	T* Allocate() {
		if (!freed_pool.empty()){
			T* object_pool = freed_pool.front();
			freed_pool.pop();
			dedicated_pool.insert(object_pool);
			return object_pool;
		}
		else {
			dedicated_pool.insert(new T);
			return *dedicated_pool.begin();
		}
	}
	T* TryAllocate() {
		if (!freed_pool.empty())
			Allocate();
		else
			return nullptr;
	}

	void Deallocate(T* object) {
		if (dedicated_pool.find(object) == dedicated_pool.end()) {
			throw invalid_argument("There is no object");
		}
		dedicated_pool.erase(object);
		freed_pool.push(object);
	}

	~ObjectPool() {
		for (auto s : dedicated_pool)
			delete s;
		while (!freed_pool.empty())
		{
			auto q = freed_pool.front();
			freed_pool.pop();
			delete q;
		}
	}

private:
	queue<T*> freed_pool;
	set<T*> dedicated_pool;
};

void TestObjectPool() {
	ObjectPool<string> pool;

	auto p1 = pool.Allocate();
	auto p2 = pool.Allocate();
	auto p3 = pool.Allocate();

	*p1 = "first";
	*p2 = "second";
	*p3 = "third";

	pool.Deallocate(p2);
	ASSERT_EQUAL(*pool.Allocate(), "second");

	pool.Deallocate(p3);
	pool.Deallocate(p1);
	ASSERT_EQUAL(*pool.Allocate(), "third");
	ASSERT_EQUAL(*pool.Allocate(), "first");

	pool.Deallocate(p1);
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestObjectPool);
	return 0;
}
