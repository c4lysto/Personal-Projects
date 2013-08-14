#pragma once
#include <thread>
#include <mutex>

// The safe thread class pretends to be a std:thread but uses a mutex to
// try and make using certain functions thread-safe such as join().(allows multiple threads to wait for this thread)
// only thread saftey is added, no extra error checking is done to try and stay closer to the standard.
// safe_thread to safe_thread copies and swaps may be added with extra security at a later time.
// L.Norri CD DRX FullSail University.
class safe_thread
{
	std::thread	unsafe_thread; // encapsulation (Is-A relationship was abandoned due to variadic template macro expansion) 
	std::mutex make_safe; // restricts access to internal thread	
public:
	// the safe_thread class cannot be used to start a thread (no specialized constructors)
	// however, you can convert an existing thread to a safe_thread.(ex: safe_thread x = thread(...);)
	// The safe_thread can be assigned threads and can cast itself to a thread if required.
	safe_thread() {}
	// copy construct a safe_thread from a (unsafe)thread
	safe_thread(std::thread&& _Other) : unsafe_thread(std::move(_Other)) {}
	// allow assignment to other threads
	std::thread& operator=(std::thread&& _Other) 
	{ 
		unsafe_thread = std::move(_Other);
		return unsafe_thread;
	}
	// A safe thread may be cast to a normal thread pointer, refrence or L-value
	operator std::thread*() { return &unsafe_thread; }
	operator const std::thread*() const { return &unsafe_thread; }
	operator std::thread&() { return unsafe_thread; }
	operator const std::thread&() const { return unsafe_thread; }
	operator std::thread&&() { return std::move(unsafe_thread); }
	operator const std::thread&&() const { return std::move(unsafe_thread); }
	
	// with our conversions overloaded, lets actually make some imposter functions with thread safety controls
	void swap(std::thread& _Other)
	{
		std::lock_guard<std::mutex> lock(make_safe);
		unsafe_thread.swap(_Other);
	}
	bool joinable() const
	{   // the reason for the const_cast is to retain the orginal std::thread function signatures exactly 		
		std::lock_guard<std::mutex> lock(*const_cast<std::mutex*>(&make_safe));
		return unsafe_thread.joinable();
	}
	void join()
	{
		std::lock_guard<std::mutex> lock(make_safe);
		unsafe_thread.join();
	}
	void detach()
	{
		std::lock_guard<std::mutex> lock(make_safe);
		unsafe_thread.detach();
	}
	std::thread::id get_id() const
	{   // the resaon for the const_cast is to retain the orginal std::thread function signatures exactly 
		std::lock_guard<std::mutex> lock(*const_cast<std::mutex*>(&make_safe));
		unsafe_thread.get_id();
	}
	static unsigned int hardware_concurrency()
	{   // mutex uneeded on static function, nothing to protect
		return std::thread::hardware_concurrency();
	}
	std::thread::native_handle_type native_handle()
	{
		std::lock_guard<std::mutex> lock(make_safe);
		unsafe_thread.native_handle();
	}

private:
	// safe_thread cannot itself be copied or initialized from another safe_thread 
	safe_thread(const safe_thread&); // not defined
	safe_thread& operator=(const safe_thread&);	// not defined
};