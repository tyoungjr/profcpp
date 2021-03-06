#include <cstddef>
import <unordered_set>;
import <functional>;
import <iostream>;

using namespace std;

class IntWrapper
{
public:
	IntWrapper(int i) : m_wrappedInt{ i } {}
	int getValue() const { return m_wrappedInt; }
	bool operator==(const IntWrapper&) const = default;// = default since C++20
private:
	int m_wrappedInt;
};

namespace std
{
	template<> struct hash<IntWrapper>
	{
		size_t operator()(const IntWrapper& x) const {
			return std::hash<int>{}(x.getValue());
		}
	};
}

int main()
{
	unordered_set<IntWrapper> s;
	s.insert(IntWrapper{ 1 });
	s.insert(IntWrapper{ 2 });

	for (const auto& element : s) {
		cout << element.getValue() << endl;
	}
}
