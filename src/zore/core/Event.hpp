#pragma once

namespace zore {

	//========================================================================
	//	Event Class
	//========================================================================

	class Event {
	public:

	private:

		//========================================================================
		//	Event Listener Class
		//========================================================================

	public:
		class Listener {
		protected:
			Listener(int priority);
			~Listener();

		private:
			virtual bool OnEvent(Event* event);

		private:
			int m_priority;
		};
	};
}