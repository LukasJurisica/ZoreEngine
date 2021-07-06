#pragma once
#include "KeyCodes.hpp"
#include <bitset>
#include <vector>
#include <string>

struct GLFWwindow;

namespace zore {

	//========================================================================
	//	Keyboard Class
	//========================================================================

	class KeyListener;
	class Keyboard {
		friend class Window;
	public:
		Keyboard(const std::string& name);
		~Keyboard();
		void RegisterListener(KeyListener* listener);
		void UnregisterListener(KeyListener* listener);

		bool GetKey(unsigned char key) const;
		void ClearState();

	private:
		static void KeyCallback(GLFWwindow* windowHandle, int key, int scancode, int action, int mods);
		static void CharCallback(GLFWwindow* windowHandle, unsigned int code);

		void OnKeyPress(unsigned char key, int mods, bool repeat);
		void OnKeyRelease(unsigned char key);
		void OnChar(char c);

		std::bitset<KEY_COUNT> keyStates;
		std::vector<KeyListener*> listeners;
	};

	//========================================================================
	//	Keyboard Listener Class
	//========================================================================

	class KeyListener {
		friend class Keyboard;
	private:
		static void RemoveWaiter(KeyListener* listener);
		static void ClaimWaiters(Keyboard* keyboard, const std::string& name);
		static void ReleaseListeners(std::vector<KeyListener*>& listeners);

		virtual void OnKeyPress(unsigned char key, int mods) { };
		virtual void OnKeyRelease(unsigned char key) { };
		virtual void OnChar(char code) { };

	protected:
		KeyListener(const std::string& name = "");
		~KeyListener();

		std::string name;
		Keyboard* keyboard;
	};
}