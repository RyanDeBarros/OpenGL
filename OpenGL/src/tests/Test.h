#pragma once

#include <functional>
#include <string>
#include <iostream>

namespace test {

	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(double deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTestPointer);

		void OnImGuiRender() override;

		template<typename T, typename... Args>
		inline void RegisterTest(const std::string& name, Args... args)
		{
			std::cout << "Registering test " << name << std::endl;
			m_Tests.push_back(std::make_pair(name, [=]() { return new T(args...); }));
		}

		void SwitchToTest(const std::string& name);
	private:
		Test*& m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
	};

}
