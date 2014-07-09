#pragma once


class CodeHistory
{
public:
	long startTime;
	long lastTime;

	std::vector<std::string> cache;

	void add(std::string text);
	
	std::string getAtTime(long time);
	std::string getCurrent();
	std::string getPrevious();

};