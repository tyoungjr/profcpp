import <type_traits>;
import <concepts>;

using namespace std;

static_assert(sizeof(void*) == 8, "Requires 64-bit compilation.");

// Using static_assert() and type traits:
//template <typename T>
//void foo(const T& t)
//{
//	static_assert(is_integral_v<T>, "T should be an integral type.");
//}

// I recommend using C++20 concepts:
//template <integral T>
//void foo(const T& t)
//{
//}

// Or, using C++20 abbreviated function template syntax:
void foo(const integral auto& t)
{
}

int main()
{
	foo(123);
	//foo(1.2);
}