#pragma once

class base_thread
{
public:
	base_thread() {};
	void kill() { die = true; }
	bool is_alive() { return alive; }

	static void __stdcall ThreadEntry(void* pUserData) {
		return ((base_thread*)pUserData)->main_loop();
	}
	virtual void main_loop() {};

protected:

	bool die = false;
	bool alive = false;	
};



