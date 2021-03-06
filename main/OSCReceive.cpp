// OSCReceive.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#if defined(__BORLANDC__) // workaround for BCB4 release build intrinsics bug
namespace std {
	using ::__strcmp__;  // avoid error: E2316 '__strcmp__' is not a member of 'std'.
}
#endif

#include "D:\OSC C++\oscpack_1_1_0\osc\OscReceivedElements.h"
#include "D:\OSC C++\oscpack_1_1_0\osc\OscPacketListener.h"
#include "D:\OSC C++\oscpack_1_1_0\ip\UdpSocket.h"


#define PORT 9999

class ExamplePacketListener : public osc::OscPacketListener {
protected:

	virtual void ProcessMessage(const osc::ReceivedMessage& m,
		const IpEndpointName& remoteEndpoint)
	{
		(void)remoteEndpoint; // suppress unused parameter warning

		try {
			// example of parsing single messages. osc::OsckPacketListener
			// handles the bundle traversal.

			if (std::strcmp(m.AddressPattern(), "/test1") == 0) {
				// example #1 -- argument stream interface
				osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
				bool a1;
				osc::int32 a2;
				float a3;
				const char *a4;
				args >> a1 >> a2 >> a3 >> a4 >> osc::EndMessage;

				std::cout << "received '/test1' message with arguments: "
					<< a1 << " " << a2 << " " << a3 << " " << a4 << "\n";

			}
			else if (std::strcmp(m.AddressPattern(), "/test2") == 0) {
				// example #2 -- argument iterator interface, supports
				// reflection for overloaded messages (eg you can call 
				// (*arg)->IsBool() to check if a bool was passed etc).
				osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
				bool a1 = (arg++)->AsBool();
				int a2 = (arg++)->AsInt32();
				float a3 = (arg++)->AsFloat();
				const char *a4 = (arg++)->AsString();
				if (arg != m.ArgumentsEnd())
					throw osc::ExcessArgumentException();

				std::cout << "received '/test2' message with arguments: "
					<< a1 << " " << a2 << " " << a3 << " " << a4 << "\n";
			}
		}
		catch (osc::Exception& e) {
			// any parsing errors such as unexpected argument types, or 
			// missing arguments get thrown as exceptions.
			std::cout << "error while parsing message: "
				<< m.AddressPattern() << ": " << e.what() << "\n";
		}
	}
};

int main()
{
	ExamplePacketListener listener;
	UdpListeningReceiveSocket s(
		IpEndpointName(IpEndpointName::ANY_ADDRESS, PORT),
		&listener);

	std::cout << "press ctrl-c to end\n";

	s.RunUntilSigInt();

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
