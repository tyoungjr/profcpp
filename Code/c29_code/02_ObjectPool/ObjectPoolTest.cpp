#include <cstddef>
import object_pool;
import <array>;
import <iostream>;
import <chrono>;
import <format>;

using namespace std;

class ExpensiveObject
{
public:
	ExpensiveObject() { /* ... */  }
	virtual ~ExpensiveObject() = default;

	// Methods to populate the object with specific information.
	// Methods to retrieve the object data.
	// (not shown)

private:
	// An expensive data member.
	array<double, 4 * 1024 * 1024> m_data;

	// Other data members (not shown)
};

using MyPool = ObjectPool<ExpensiveObject>;

shared_ptr<ExpensiveObject> getExpensiveObject(MyPool& pool)
{
	// Obtain an ExpensiveObject object from the pool.
	auto object{ pool.acquireObject() };
	 
	// Populate the object. (not shown)

	return object;
}

void processExpensiveObject(ExpensiveObject* /*object*/)
{
	// Process the object. (not shown)
}

int main()
{
	const size_t NumberOfIterations{ 500'000 };

	cout << "Starting loop using pool..." << endl;
	MyPool requestPool;
	auto start1{ chrono::steady_clock::now() };
	for (size_t i{ 0 }; i < NumberOfIterations; ++i) {
		auto object{ getExpensiveObject(requestPool) };
		processExpensiveObject(object.get());
	}
	auto end1{ chrono::steady_clock::now() };
	auto diff1{ end1 - start1 };
	cout << format("{}ms\n", chrono::duration<double, milli>(diff1).count());


	cout << "Starting loop using new/delete..." << endl;
	auto start2{ chrono::steady_clock::now() };
	for (size_t i{ 0 }; i < NumberOfIterations; ++i) {
		auto object{ new ExpensiveObject{} };
		processExpensiveObject(object);
		delete object; object = nullptr;
	}
	auto end2{ chrono::steady_clock::now() };
	auto diff2{ end2 - start2 };
	cout << format("{}ms\n", chrono::duration<double, milli>(diff2).count());
}
