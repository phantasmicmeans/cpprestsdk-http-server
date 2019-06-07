
#include "server.h"
#include <iostream>
#include <unistd.h>
#include <stdio.h>

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
		std::cout << "message.extrace_json().get() Exception : " <<  e.what()  << endl;
		response_value["response"] = json::value::string(e.what());
		message.reply(status_codes::BadRequest, response_value);
		return;
	}

	if(request_json.is_null() || request_json.size() > 4) {
		std::cout << "Received_Json_Value : "<< request_json << " => Bad Request" << endl;
		message.reply(status_codes::BadRequest, response_value);
		return;
	}

	if(request_json.is_object() && request_json.size() == 4) {
		bool hasApplicationId = request_json.has_field("app_type");
		bool hasResourceId = request_json.has_field("resource_id");
		bool hasCallerHandleId = request_json.has_field("caller_handle_id");
		bool hasNewHandleId = request_json.has_field("new_handle_id");

		if(!hasApplicationId || !hasResourceId || !hasCallerHandleId || !hasNewHandleId) {
			std::cout << "Received_Json_Value From : "<< request_json << " => Bad Request" << endl;
			message.reply(status_codes::BadRequest, response_value);
			return;
		}else {
			json::value response_success_value;
			json::value json_value;
			json_value["app_type"] = request_json.at("app_type");
			json_value["resource_id"] = request_json.at("resource_id");
			json_value["caller_handle_id"] = request_json.at("caller_handle_id");
			json_value["new_handle_id"] = request_json.at("new_handle_id");

			std::cout << "Received_Json_Value From : "<< json_value << " => successfully received" << endl;
			std::cout << endl;

			string app_type = request_json.at("app_type").as_string();
			string resource_id = request_json.at("resource_id").as_string();
			string caller_handle_id = request_json.at("caller_handle_id").as_string();
			string new_handle_id = request_json.at("new_handle_id").as_string();

			string shellCmd = "./test_bash.sh "+ app_type +" "+ resource_id + " " + caller_handle_id + " " + new_handle_id;
			std::cout << "shellCmd : " << shellCmd << endl;
//			system(shellCmd.c_str());
			/*
			 * 여기에 추가
			 */
			response_success_value["result"] = json::value::string("successfully received");
			response_success_value["requestd_value"] = json_value;
			message.reply(status_codes::OK, response_success_value);
			return;
		}
	} else
		message.reply(status_codes::BadRequest, response_value);

};
