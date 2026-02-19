#include <string>
#include <curl/curl.h>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
  s->append((char*)contents, size * nmemb);
  return size * nmemb;
}

std::string get_news(std::string url, std::string token) {
  CURL* curl = curl_easy_init();
  std::string response;
  struct curl_slist *headers = NULL;
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); 
    std::string auth_header = "Authorization: Bearer " + token;
    headers = curl_slist_append(headers, auth_header.c_str()); 
    headers = curl_slist_append(headers, "Accept: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    CURLcode res = curl_easy_perform(curl);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res == CURLE_OK) {
      return response;
    }
  }
  return "Error";
}
