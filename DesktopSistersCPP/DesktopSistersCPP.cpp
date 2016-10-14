// natTestProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <chrono>
#include <thread>
#include <future>

#include "Sisters.h"

using namespace std;

int main()
{
	unique_ptr<Sisters> sisters(new Sisters());
	while (true)
	{
		sisters->Pulse();
		std::this_thread::sleep_for(std::chrono::milliseconds{10});
	}
}



