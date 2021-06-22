#include "http.h"
#include <curl/curl.h>
#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "crypt32.lib")

namespace http
{
	std::size_t write_function(void* buffer, std::size_t size, std::size_t nmemb, void* userp)
	{
		reinterpret_cast<std::string*>(userp)->append(reinterpret_cast<const char*>(buffer), size * nmemb);

		return size * nmemb;
	}
	std::string get(const std::string& url)
	{
		CURL* handle = curl_easy_init();

		std::string data;

		curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_function);
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, &data);

		auto result = curl_easy_perform(handle);

		curl_easy_cleanup(handle);

		if (result == CURLE_OK)
			return data;

		return "";
	}

}