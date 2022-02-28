#include "WebRequest.h"

#define RSPrint(x) std::cout << x << std::endl

#pragma region WebTools
std::string WebTools::__escapeAll(std::regex regEx, std::string str) {
	std::string encoded = "";

	for (std::string::size_type i = 0; i < str.size(); i++) {
		if (std::regex_match(str.substr(i, 1), regEx)) {
			encoded += str[i];
			continue;
		}

		std::ostringstream ostr;
		ostr << '%' << std::uppercase << std::hex << (0xff & str[i]);

		encoded += ostr.str();
	}

	return encoded;
}

std::string WebTools::EncodeURI(std::string uri) {
	std::regex regEx("[A-Za-z0-9;,\\/?:@&=+$\\-\\_.!~*'()#]");
	std::string encoded = __escapeAll(regEx, uri);

	return encoded;
}

std::string WebTools::EncodeURIComponent(std::string uri) {
	std::regex regEx("[A-Za-z0-9\\-\\_.!~*'()]");
	std::string encoded = __escapeAll(regEx, uri);

	return encoded;
}
#pragma endregion

#pragma region Curl Callbacks

int __curlProgressCallback(WebResponse* clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
	if (dltotal == 0.0) return 0;

	clientp->progress = (float)dlnow / (float)dltotal;

	RSPrint(clientp->progress);

	return 0;
}

size_t __curlWriterCallback(char* buffer, size_t size, size_t nmemb, WebResponse* clientp) {
	clientp->body += buffer;

	return size * nmemb;
}

size_t __curlHeaderCallback(char* buffer, size_t size, size_t nitems, WebResponse* clientp) {
	clientp->headers += buffer;

	return size * nitems;
}

#pragma endregion

void WebRequest::__startRequest(WebForm* form, WebResponse* response) {
	// Create CURL request
	CURL* curl = curl_easy_init();

	if (curl) {
		// Start configuration
		std::string requestUrl = url;
		curl_easy_setopt(curl, CURLOPT_MAXREDIRS, (long)maxRedirects);
		curl_easy_setopt(curl, CURLOPT_MAXLIFETIME_CONN, (long)maxTime);
		curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, TRUE);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, TRUE);
		curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, acceptEncoding.c_str());

		// Pass data
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, MethodToString().c_str());
		if (cookies != "") {
			curl_easy_setopt(curl, CURLOPT_COOKIE, cookies.c_str());
		}
		struct curl_slist* headersList = NULL;
		headers.ParseToCurlList(headersList, true);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headersList);

		// Prepare URL
		if (form) {
			switch (method) {
			case HttpMethod::HTTP_GET:
				if (requestUrl.find('?') != std::string::npos)
					requestUrl += "&" + form->Parse();
				else
					requestUrl += "?" + form->Parse();
				break;
			case HttpMethod::HTTP_POST:
			case HttpMethod::HTTP_PUT:
			case HttpMethod::HTTP_DELETE:
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, form->Parse().c_str());
				break;
			}
		}
		curl_easy_setopt(curl, CURLOPT_URL, requestUrl.c_str());

		// Tracking progress
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
		curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, __curlProgressCallback);
		curl_easy_setopt(curl, CURLOPT_XFERINFODATA, response);

		// Tracking body data
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, __curlWriterCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

		// Tracking headers data
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, __curlHeaderCallback);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, response);

		// Start CURL request
		CURLcode res = curl_easy_perform(curl);

		if (res == CURLE_OK) {
			// Tracking status data
			long statusCode, timeElapsed;
			char *responseURL, *responseMIME;

			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
			curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &timeElapsed);
			curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &responseURL);
			curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &responseMIME);

			response->code = statusCode;
			response->timeElapsed = timeElapsed;
			if (responseMIME != NULL) response->mime = responseMIME;
			if (responseURL != NULL) response->url = responseURL;
			response->isDone = true;
		} else {
			std::string error = curl_easy_strerror(res);

			response->code = 10401;
			response->body = "CURL internal error: " + error;
			response->mime = "text/plain";
			response->isDone = true;
		}

		curl_easy_cleanup(curl);
		curl_slist_free_all(headersList);

		headersList = NULL;
		curl = NULL;
	}
	else {
		response->code = 10400;
		response->body = "CURL init unknown error.";
		response->mime = "text/plain";
		response->isDone = true;
	}
}
