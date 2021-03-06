#include "pch.h"
#include <string>
#include <list>
#include <thread>
#include <mutex>
#include "interval.h"
#include "xmlParse.h"
#include "FileService.h"
using namespace std;

void defineIntervals(xmlParse & xmlParser, list<interval>& interList);
void findPrimes(const interval, list<unsigned int>&, mutex&);
inline bool is_prime(unsigned int);

int main(int argc, char* argv[])
{
	const char * fileName = "file.xml";
	FileService fileService{};
	string content = fileService.loadFileData(fileName);
	xmlParse xmlParser(content);

	list<interval> intervalList;
	defineIntervals(xmlParser, intervalList);

	list<unsigned int> resultList;
	list<thread> threads;
	mutex access;
	for (auto iter : intervalList)
		threads.push_back(thread(findPrimes, iter, ref(resultList), ref(access)));
	for (auto &thr : threads)
		if (thr.joinable()) thr.join();

	resultList.unique();
	string primeXMLString = xmlParser.makeXMLString(resultList);
	fileService.insertToFileBefore(fileName, primeXMLString, "root");

	return 0;
}

void defineIntervals(xmlParse & xmlParser, list<interval>& interList)
{
	try
	{
		string intervals = xmlParser.find("intervals");
		xmlParser.setString(intervals);

		list<string> stringsList = xmlParser.findAll("interval");
		auto left = 0, right = 0;
		for (auto iter = stringsList.begin(); iter != stringsList.end(); iter++)
		{
			xmlParser.setString(*iter);
			left = atoi(xmlParser.find("low").c_str());
			right = atoi(xmlParser.find("high").c_str());
			interList.push_back(interval(left, right));
		}
	}
	catch (const MyException & exp)
	{
		cerr << exp.getMessage() << endl;
	}
	catch (const std::exception & exp)
	{
		cerr << exp.what() << endl;
	}
	catch (...)
	{
		cerr << "Unknown error!\n";
	}
}

void findPrimes(const interval inter, list<unsigned int>& result, mutex& access)
{
	access.lock();
	cout << "Thread " << this_thread::get_id() << ":\t started \t(" << inter.getLeft() << " - " << inter.getRight() << ')' << endl;
	access.unlock();

	for (unsigned int idx = inter.getLeft(); idx < inter.getRight(); idx++)
	{
		if (is_prime(idx))
		{
			access.lock();
			result.push_back(idx);
			access.unlock();
		}
	}

	access.lock();
	cout << "Thread " << this_thread::get_id() << ":\t finish \t(" << inter.getLeft() << " - " << inter.getRight() << ')' << endl;
	access.unlock();
}

inline bool is_prime(unsigned int number)
{
	for (auto i = 2; i < sqrt(number) + 1; i++)
		if (number%i == 0)
			return false;
	return true;
}

