#pragma once

#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

// HTML nor CSS explode when you do something stupid.
class ErrorHolder {
public:

	vector<string> Warnings = vector<string>();
	vector<string> Errors = vector<string>();

	void Warn(string message) {
		for (string msg : Warnings) {
			if (msg == message)
				return;
		}

		Warnings.push_back(message);
	}

	void Error(string message) {
		for (string msg : Errors) {
			if (msg == message)
				return;
		}

		Errors.push_back(message);
	}

	void Print() {
		for (string message : Errors) {
			std::cout << "[Error]: " << message << "\n";
		}

		for (string message : Warnings) {
			std::cout << "[Warning]: " << message << "\n";
		}
	}
};