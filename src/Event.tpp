#pragma once
#include "pch.h"

namespace BoardGame {
	template<typename... agruments>
	class Event {
	public:
		void Invoke(agruments... args) {
			for (auto func : functions) {
				func(args...);
			}
		}
		void AddListener(void func(agruments...)) {
			functions.push_back(func);
		}
		void RemoveListener(void func(agruments...)) {
			auto iter = std::find(functions.begin(), functions.end(), func);

			if (iter != functions.end())
				functions.erase(iter);
		}
	private:
		std::vector<void(*)(agruments...)> functions;
	};
}