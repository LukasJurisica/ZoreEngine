#pragma once

namespace zore {

	//========================================================================
	// Console
	//========================================================================

	class Console {
	public:
		Console();
		~Console();
		enum class Colour { White, Gray, Blue, Green, Red, Yellow, Cyan };
		static void SetTextColor(Colour c);

	private:
		static Colour activeColour;
	};
}