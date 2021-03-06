import <iostream>;
import <string>;
import <exception>;
import <source_location>;
import <format>;

using namespace std;

class MyException : public exception
{
public:
	MyException(string message,
		source_location location = source_location::current())
		: m_message{ move(message) }
		, m_location{ move(location) }
	{
	}

	const char* what() const noexcept override { return m_message.c_str(); }

	virtual const source_location& where() const noexcept { return m_location; }

private:
	string m_message;
	source_location m_location;
};

void doSomething()
{
	throw MyException{ "Throwing MyException." };
}

int main()
{
	try {
		doSomething();
	} catch (const MyException& e) {
		const auto& location{ e.where() };
		cerr << format("Caught: '{}' at line {} in {}.",
			e.what(), location.line(), location.function_name()) << endl;
	}
}
