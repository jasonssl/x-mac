// x_mac.cpp :
// A demo project to change the mac address of the wireless card.
//
// JasonSSL (@JasonSSL)
// jason.sslim@gmail.com
// 

#include <stdlib.h>
#include <iostream>
#include <time.h>

using namespace System;
using namespace Microsoft::Win32;

int main(int argc, char *argv[])
{
	srand((unsigned int)time(0));
	if (argc != 2)
	{
		// Usage
		Console::WriteLine("Usage: x_mac [-n] [-r]\n");
		Console::WriteLine("\t-n\t\tCreate a new MAC address for windows machine.");
		Console::WriteLine("\t-r\t\tReset mac address back to original MAC address.");

		return -1;
	}
	else
	{
		// Constructing the name of the key in a valid root
		// My complete path >> HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Control\Class\{4d36e972-e325-11ce-bfc1-08002be10318}\0000
		String^ userRoot = "HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\Control\\Class\\{4d36e972-e325-11ce-bfc1-08002be10318}";
		String^ subKey = "0000"; // Normally, the subkey is vary on machines
		String^ keyName = String::Concat(userRoot, "\\", subKey);
		
		if (strcmp(argv[1], "-r") == 0)
		{
			// Clear the data value inside the network address value
			Console::WriteLine("Resetting MAC address back to original ...");
			Registry::SetValue(keyName, "NetworkAddress", "");

			// Reset the wireless adapater
			system("netsh interface set interface \"Wi-Fi\" DISABLED");
			system("netsh interface set interface \"Wi-Fi\" ENABLED");
			return 0;
		}
		else if (strcmp(argv[1], "-n") == 0)
		{
			Console::WriteLine("Generating new MAC address");

			String^ newMac;

			// Some adapters (especially Wi-Fi cards) does not allow changes to MAC Address,
			// if the first two octet does not equals to D2, D6, DA, DE
			array<String^>^ secOctetArray = { "2", "6", "A", "E" };

			array<String^>^ appendArray = {
				"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
				"A", "B", "C", "D", "E", "F" };

			newMac = appendArray[rand() % 16];
			newMac += secOctetArray[(rand() % 4)];

			for (int i = 0; i < 10; i++)
			{
				newMac += appendArray[rand() % 16];
				//Console::WriteLine("appendArray({0}): {1}", i, appendArray[val]);
			}

			Console::WriteLine("New Mac Address: {0}", newMac);
			Registry::SetValue(keyName, "NetworkAddress", newMac);

			// Reset the wireless adapater
			system("netsh interface set interface \"Wi-Fi\" DISABLED");
			system("netsh interface set interface \"Wi-Fi\" ENABLED");
			return 0;
		}
		else 
		{
			Console::WriteLine("Usage: x_mac [-n] [-r]\n");
			Console::WriteLine("\t-n\t\tCreate a new mac address for windows machine.");
			Console::WriteLine("\t-r\t\tReset mac address back to original mac.");

			return -1;
		}
	}
}