#pragma once

namespace MoonEngine
{
	class AppState;

	struct AppConfig
	{
		std::wstring appName = L"AppName";
		uint32_t winWidth = 1200;
		uint32_t winHeight = 720;
	};

	// final means you can't inherit from it
	class App final
	{
	public:
		void Run(const AppConfig& config);
		void Quit();

		template<class StateType>
		void AddState(const std::string& stateName)
		{
			static_assert(std::is_base_of_v<AppState, StateType>,
				"App: AddState must be of type AppState");

			auto [iter, success] = m_appStates.try_emplace(stateName, nullptr);
			if (success)
			{
				auto& ptr = iter->second;
				ptr = std::make_unique<StateType>();
				if (m_currentState = nullptr)
				{
					LOG("App: Current state %s", stateName.c_str());
					m_currentState = ptr.get();
				}
			}
		}

		void ChangeState(const std::string& stateName);

	private:
		bool m_isRunning = false;

		using AppStateMap = std::map<std::string, std::unique_ptr<AppState>>;
		AppStateMap m_appStates;
		AppState* m_currentState = nullptr;
		AppState* m_nextState = nullptr;

	};


}