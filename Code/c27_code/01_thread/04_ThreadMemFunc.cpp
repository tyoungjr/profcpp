import <thread>;
import <iostream>;

using namespace std;

class Request
{
public:
	Request(int id) : m_id{ id } { }

	void process()
	{
		cout << "Processing request " << m_id << endl;
	}

private:
	int m_id;
};

int main()
{
	Request req{ 100 };
	thread t{ &Request::process, &req };

	t.join();
}
