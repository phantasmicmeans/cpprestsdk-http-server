
#include "server.h"

using namespace std;
using namespace web;
using namespace utility;
using namespace http;
using namespace web::http::experimental::listener;

Server::Server(utility::string_t url) : m_listener(url) {
	string get = "GET";
	string post = "POST";
	m_listener.support(get, std::bind(&Server::handle_get, this, std::placeholders::_1));
	m_listener.support(post, std::bind(&Server::handle_post, this, std::placeholders::_1));
}

void Server::handle_get(http_request message) {
	std::cout << message.to_string() << endl;
	message.reply(status_codes::OK);
};

void Server::handle_post(http_request message) {
	std::cout << message.to_string() << endl;
	json::value request_json;

	json::value response_value;
	response_value["response"] = json::value::string("input을 확인하세요.");

	try{
		request_json = message.extract_json().get();
	}catch(json::json_exception &e) {
		std::cout << "message.extrace_json().get() Exception From " << message.remote_address() << ": " <<  e.what()  << endl;
		response_value["response"] = json::value::string(e.what());
		message.reply(status_codes::BadRequest, response_value);
		return;
	}

	if(request_json.is_null() || request_json.size() > 2) {
		std::cout << "Received_Json_Value From "<< message.remote_address() << ": "<< request_json << " => Bad Request" << endl;
		message.reply(status_codes::BadRequest, response_value);
		return;
	}

	if(request_json.is_object() && request_json.size() == 2) {
		bool hasHandleId = request_json.has_field("handleId");
		bool hasResourceId = request_json.has_field("resourceId");

		if(!hasHandleId || !hasResourceId) {
			std::cout << "Received_Json_Value From "<< message.remote_address() << ": "<< request_json << " => Bad Request" << endl;
			message.reply(status_codes::BadRequest, response_value);
			return;
		}else {
			json::value json_value;
			json_value["resourceId"] = request_json.at("resourceId");
			json_value["handleId"] = request_json.at("handleId");
			std::cout << "Received_Json_Value From "<< message.remote_address() << ": "<< json_value << " => successfully received" << endl;

			/*
			 * 여기에 추가
			 */
			response_value["response"] = json::value::string("successfully received");
			message.reply(status_codes::OK, response_value);
			return;
		}
	} else
		message.reply(status_codes::BadRequest, response_value);
};
