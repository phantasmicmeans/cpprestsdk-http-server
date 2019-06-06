#include "server.h"

using namespace std;
using namespace web;
using namespace utility;
using namespace http;
using namespace web::http::experimental::listener;

std::unique_ptr<Server> g_http;

void on_initialize(const string_t& address) {

	uri_builder uri(address);
	uri.set_path("load/show");

	auto addr = uri.to_uri().to_string();
	//auto addr = uri.m_uri.m_host;
	g_http = std::unique_ptr<Server>(new Server(addr));
	g_http->open().wait();

	ucout << utility::string_t("Listening for request at: ") << addr << std::endl;
	return ;
}

void on_shutdown () {
	g_http->close().wait();
	return;
}

int main(int argc, char_t * argv[]) {
	utility::string_t port = U("8888");
//	if(argc==2) {
//		port = argv[1];
//	}

	utility::string_t address = U("http://localhost:");
	address.append(port);

	on_initialize(address);
	std::cout << "Press Enter to Exit " << std::endl;
	std::string line;
	std::getline(std::cin, line);

	on_shutdown();
	return 0;
}
