//
// Created by 123ja on 8/23/2025.
////
// ClockCallable.cpp - Native clock function implementation
//

#include "ClockCallable.h"
#include <chrono>

int ClockCallable::arity()
{
	return 0;
}

Obj ClockCallable::call(Interpreter& interpreter, std::vector<Obj> arguments)
{
	auto now = std::chrono::system_clock::now();
	auto duration = now.time_since_epoch();
	auto seconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	return Obj(static_cast<double>(seconds) / 1000.0);
}

std::string ClockCallable::toString()
{
	return "<native fn>";
}