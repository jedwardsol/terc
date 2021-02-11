
//c:\sdk\googletest\googletest\include\gtest\internal\gtest-port.h(1389): warning C26495: Variable 'testing::internal::Mutex::critical_section_init_phase_' is uninitialized. Always initialize a member variable (type.6).
//c:\sdk\googletest\googletest\include\gtest\internal\gtest-port.h(1389): warning C26812: The enum type 'testing::internal::Mutex::StaticConstructorSelector' is unscoped. Prefer 'enum class' over 'enum' (Enum.3).
//c:\sdk\googletest\googletest\include\gtest\gtest-test-part.h(63): warning C26812: The enum type 'testing::TestPartResult::Type' is unscoped. Prefer 'enum class' over 'enum' (Enum.3).

// c:\sdk\googletest\googletest\include\gtest\gtest-matchers.h(315): warning C26439: This kind of function may not throw. Declare it 'noexcept' (f.6).


#pragma warning(push)
#pragma warning(disable:  26495  26812 26439)

#include <gtest/gtest.h>

#pragma warning(pop)


#if defined _DEBUG
#pragma comment(lib,"gtestd")
#else
#pragma comment(lib,"gtest")
#endif
