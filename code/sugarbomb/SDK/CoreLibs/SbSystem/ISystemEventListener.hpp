/// @file
/// @brief system event listener interface - attach an instance of some class which implements this interface to the system to receive events from it

namespace sbe
{

struct ISystemEventListener
{
	/// Will be called when a system error happens
	/// @param asMsg - pre-formatted message string
	virtual void OnError(const char *asMsg) = 0;
};

}; // namespace sbe