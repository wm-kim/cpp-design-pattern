#include "stdafx.h"

struct Pingable
{
	virtual wstring ping(const wstring& message) = 0;
};

struct Pong : Pingable
{
	wstring ping(const wstring& message) override
	{
		return messsage + L" pong";
	}
};

void tryit(Pingable& pp)
{
	wcout << pp.ping(L"ping") << "\n";
}

int main()
{
	Pong pp;
	for (size_t i = 0; i < 3; i++)
	{
		tryit(pp);
	}
}