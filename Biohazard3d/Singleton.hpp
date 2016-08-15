


#ifndef _BHD_SINGLETON_H
#define _BHD_SINGLETON_H
#pragma once

namespace bhd
{
	//C++ 11 singleton
	template <typename T>
	class Singleton
	{
	public:
		static T & instance(void *data = nullptr)
		{
			// Since it's a static variable, if the class has already been created,
			// It won't be created again.
			// And it **is** thread-safe in C++11.

			static T hansolo(data);

			// Return a reference to our instance.
			return hansolo;
		}

		// delete copy and move constructors and assign operators
		Singleton(T const&) = delete;             // Copy construct
		Singleton(T&&) = delete;                  // Move construct
		T& operator=(T const&) = delete;  // Copy assign
		T& operator=(T &&) = delete;      // Move assign

	protected:
		Singleton()	{}
		~Singleton(){}
	};
}


#endif //_BHD_SINGLETON_H