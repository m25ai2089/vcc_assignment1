#include <iostream>
#include <string>
#include "httplib.h"

int main()
{
	httplib::Server svr;

	svr.Get("/health", [](const httplib::Request&, httplib::Response& res) {
		res.set_content("{\"status\"ok\",\"service\":\"inventory\"}","application/json");

	});

	svr.Get("/items", [](const httplib::Request&, httplib::Response& res) {
		std::string body = 
			"{"
			"\"service\":\"inventory\","
			"\"items\":[{\"id\":1,\"name\":\"apple\"},{\"id\":2,\"name\":\"banana\"}]"
			"}";
		res.set_content(body, "application/json");
	});

	std::cout << "Inventory service running on port 8081\n";
	svr.listen("0.0.0.0", 8081);

	return 0;
}
