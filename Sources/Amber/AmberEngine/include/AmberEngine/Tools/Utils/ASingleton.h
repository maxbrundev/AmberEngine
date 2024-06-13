#pragma once

template <typename T>
class ASingleton
{
public:
	//  Scott Meyers Singleton with lazy initialization, thread safe since C++ 11.

	// Static function objects are initialized when control flow hits the function for the first time.
	// The lifetime of function static variables begins the first time the program flow encounters the declaration and ends at program termination.
	// If control enters the declaration concurrently while the variable is being initialized, the concurrent execution shall wait for completion of the initialization.
	static T& Instance()
	{
		// Magic static
		static T instance;
		
		return instance;
	}

	ASingleton(const ASingleton&) = delete;
	ASingleton& operator= (const ASingleton&) = delete;

	ASingleton(ASingleton&&) = delete;
	ASingleton& operator=(ASingleton&&) = delete;

protected:
	ASingleton() = default;
	virtual ~ASingleton() = default;
};
