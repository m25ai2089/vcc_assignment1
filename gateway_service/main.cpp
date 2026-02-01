#include <iostream>
#include <string>
#include "httplib.h"

int main() 
{
	httplib::Server svr;

	svr.Get("/health", [](const httplib::Request&, httplib::Response& res) {
		res.set_content("{\"status\":\"ok\",\"service\":\"gateway\"}", "application/json");		
	});

	svr.Get("/proxy-items", [](const httplib::Request&, httplib::Response& res){
		httplib::Client cli("192.168.56.4", 8081); // inventory service
		auto r = cli.Get("/items");

		if(!r)	{
			res.status = 502;
			res.set_content("{\"error\":\"inventory unreachable\"}", "application/json");
			return;
		}

		std::string body = std::string("{\"service\":\"gateway\",\"data\":") + r->body + "}";
		res.set_content(body, "application/json");

	});

	std::cout << "Gateway service running on port 8080\n";
	svr.listen("0.0.0.0", 8080);

	return 0;
}
