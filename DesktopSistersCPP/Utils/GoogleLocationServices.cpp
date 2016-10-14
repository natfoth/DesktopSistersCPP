#include "stdafx.h"
#include "GoogleLocationServices.h"



using namespace std;

GoogleLocationServices::GoogleLocationServices(bool shouldUseHttps)
{
	useHttps = shouldUseHttps;
}


GoogleLocationServices::~GoogleLocationServices()
{
}

string GoogleLocationServices::UrlProtocolPrefix()
{
	if (useHttps)
		return "https://";
	else
		return "http://";
}

string GoogleLocationServices::APIUrlRegionFromLatLong()
{
	return UrlProtocolPrefix() + API_REGION_FROM_LATLONG;
}

string GoogleLocationServices::APIUrlLatLongFromAddress(string address)
{
	auto urladdress = address;
	for (size_t pos = urladdress.find(' ');
	pos != string::npos;
		pos = urladdress.find(' ', pos))
	{
		urladdress.replace(pos, 1, "%20");
	}

	return "GET /maps/api/geocode/xml?address=" + urladdress + "&sensor=false HTTP/1.1\r\nConnection: close\r\n\r\n";
}


MapPoint * GoogleLocationServices::GetLatLongFromAddress(string address)
{

	string requestString = APIUrlLatLongFromAddress(address);

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cout << "WSAStartup failed.\n";
		system("pause");
	}
	SOCKET Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct hostent *host;
	host = gethostbyname("maps.googleapis.com");
	SOCKADDR_IN SockAddr;
	SockAddr.sin_port = htons(80);
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = *reinterpret_cast<unsigned long*>(host->h_addr);
	cout << "Connecting...\n";
	if (connect(Socket, reinterpret_cast<SOCKADDR*>(&SockAddr), sizeof(SockAddr)) != 0) {
		cout << "Could not connect";
		system("pause");
	}
	cout << "Connected.\n";
	send(Socket, requestString.c_str(), strlen(requestString.c_str()), 0);
	char buffer[10000];
	int nDataLength;
	while ((nDataLength = recv(Socket, buffer, 10000, 0)) > 0) {
		int i = 0;
		while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r') {
			//cout << buffer[i];
			i += 1;
		}
	}
	closesocket(Socket);
	WSACleanup();

	string str(buffer);

	string latString = "";
	string lngString = "";

	auto foundStartPosition = str.find("<lat>");
	auto foundEndPosition = str.find("</lat>");
	if (foundStartPosition != std::string::npos && foundEndPosition != std::string::npos)
	{
		foundStartPosition += 5;
		latString = str.substr(foundStartPosition, foundEndPosition - foundStartPosition);
	}

	foundStartPosition = str.find("<lng>");
	foundEndPosition = str.find("</lng>");
	if (foundStartPosition != std::string::npos && foundEndPosition != std::string::npos)
	{
		foundStartPosition += 5;
		lngString = str.substr(foundStartPosition, foundEndPosition - foundStartPosition);
	}


	return new MapPoint(float(atof(latString.c_str())), float(atof(lngString.c_str())));
}