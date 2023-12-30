#include "zore/networking/Request.hpp"
#include "zore/networking/Socket.hpp"
#include "zore/debug/Debug.hpp"

#define MAX_RESPONSE_SIZE 1024

namespace zore {

	std::string Request::Make(const std::string& url, HTTPMethod method, int port) {
		Logger::Log("HTTP GET request made to: " + url);

		// Determine if http(s):// is included in the url
		int offset = url.find_first_of("://");
		offset = (offset == url.npos) ? 0 : offset + 3;

		// Split the url into a domain and page
		int page_index = url.find_first_of("/", offset);
		std::string domain = url.substr(offset, page_index - offset);
		std::string page = (page_index == url.npos) ? "/" : url.substr(page_index);

		// Create a socket to make the request
		ActiveSocket socket(domain, port, Protocol::TCP);

		if (socket.IsValid()) {
			// Build the request
			std::string HTTP_METHOD_TO_STRING[] = { "GET", "POST" };
			std::string request = HTTP_METHOD_TO_STRING[static_cast<int>(method)];
			request += " " + page + " HTTP/1.1\r\nHost:" + domain + "\r\n\r\n";

			// Send the packet
			int request_size = static_cast<int>(request.length());
			if (!socket.SendPacket(request.c_str(), &request_size))
				Socket::PrintLastError("SendPacket");

			std::string result;
			char buffer[MAX_RESPONSE_SIZE];
			int response_size;

			do {
				response_size = MAX_RESPONSE_SIZE;
				if (!socket.ReceivePacket(buffer, &response_size)) {
					Socket::PrintLastError("ReceivePacket");
					break;
				}
				result += std::string(buffer, response_size);
				size_t end_index = result.rfind("\r\n\r\n");
				if (end_index == result.size() - 2) {
					result.resize(result.size() - 2);
					break;
				}
			} while (response_size != 0);

			return result;
		}
	}
}