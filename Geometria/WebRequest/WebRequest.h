#pragma once

#ifndef GEOMETRIA_H
#include "geometria.h"
#endif

#include "curl/curl.h"
#include <regex>

class WebTools {
private:
	// Pro tip: don't repeat yourself.
	static std::string __escapeAll(std::regex regEx, std::string str);
public:
	// Encodes an string into URL format.
	static std::string EncodeURI(std::string uri);
	//static std::string DecodeURI(std::string uri);

	// Encodes an string into URI format.
	static std::string EncodeURIComponent(std::string uri);
	//static std::string DecodeURIComponent(std::string uri);

	//static std::string __unescapeAll(std::regex regEx, std::string str);
};

struct WebFormField {
	std::string fieldName;
	std::string value;

	WebFormField(std::string fieldName, std::string value) {
		this->fieldName = fieldName;
		this->value = value;
	}
};

struct WebForm {
	std::list <WebFormField> data;

	std::string __parser(std::string name, std::string value, bool toHeader = false) {
		if (toHeader)
			return name + ": " + WebTools::EncodeURIComponent(value);

		return WebTools::EncodeURIComponent(name) + "=" + WebTools::EncodeURIComponent(value);
	}

	// Adds a new field.
	void AddField(std::string fieldName, std::string value) {
		WebFormField newData = WebFormField(fieldName, value);

		data.push_front(newData);
	}

	// Parses all the fields to URI format.
	std::string Parse(bool isHeader = false) {
		std::list <WebFormField>::iterator it;
		std::string result = "";
		bool isFirst = true;

		for (it = data.begin(); it != data.end(); it++) {
			result += (isFirst ? "" : (isHeader ? "\n" : "&")) + __parser(it->fieldName, it->value, isHeader);

			if (isFirst) isFirst = false;
		}

		return result;
	}

	// Parses all the fields to a Curl list.
	void ParseToCurlList(curl_slist* list, bool isHeader = false) {
		std::list <WebFormField>::iterator it;

		for (it = data.begin(); it != data.end(); it++) {
			std::string strData = __parser(it->fieldName, it->value, isHeader);
			list = curl_slist_append(list, strData.c_str());
		}
	}
};

struct WebResponse {
	unsigned int code;
	float timeElapsed;
	std::string body, headers, mime, url;
	bool isDone;

	float progress;
};

class WebRequest {
private:
	void __startRequest(WebForm* form, WebResponse* response);
public:
	enum class HttpMethod { HTTP_GET, HTTP_HEAD, HTTP_POST, HTTP_PUT, HTTP_DELETE };

	std::string url, cookies, acceptEncoding = "";
	HttpMethod method;
	WebForm headers;
	unsigned int status, maxRedirects = 10;
	unsigned long maxTime = 60L;

	WebRequest(std::string url) {
		this->url = url;
		method = HttpMethod::HTTP_GET;
	}

	WebRequest(std::string url, HttpMethod method) {
		this->url = url;
		this->method = method;
	}

	// Sets a new request header.
	void SetRequestHeader(std::string name, std::string value) {
		headers.AddField(name, value);
	}

	// Converts the current method to a string.
	std::string MethodToString() {
		switch (method) {
			case HttpMethod::HTTP_GET: return "GET";
			case HttpMethod::HTTP_HEAD: return "HEAD";
			case HttpMethod::HTTP_POST: return "POST";
			case HttpMethod::HTTP_PUT: return "PUT";
			case HttpMethod::HTTP_DELETE: return "DELETE";
		}

		return "GET";
	}

	// Starts a web request.
	void SendWebRequest(WebResponse* response) {
		this->__startRequest(NULL, response);
	}

	// Starts a web request.
	void SendWebRequest(WebResponse* response, WebForm form) {
		this->__startRequest(&form, response);
	}
};
