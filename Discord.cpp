// Example usage of the Discord Webhook C++ library
//#include "Discord.h"
//
//int main() {
//    const std::string webhook_url = "https://discord.com/api/webhooks/YOUR_WEBHOOK_ID/YOUR_WEBHOOK_TOKEN";
//    DiscordWebhook webhook(webhook_url);
//
//    webhook.setContent("Hello from C++!");
//    webhook.setUsername("Bot");
//    webhook.setAvatarUrl("https://example.com/avatar.png");
//
//    DiscordEmbed embed("Embed Title", "Embed Description");
//    embed.setColor(0x00FF00);
//    embed.setTimestamp();
//
//    webhook.addEmbed(embed);
//
//    webhook.execute();
//
//    return 0;
//}


#include "Discord.h"
#include <windows.h>
#include <winhttp.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <chrono>

#pragma comment(lib, "winhttp.lib")

// Utility function to convert std::string to std::wstring
std::wstring stringToWstring(const std::string& str) {
    int len;
    int slength = (int)str.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, 0, 0);
    std::wstring r(len, L'\0');
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, &r[0], len);
    return r;
}

// Utility function to get current timestamp in ISO 8601 format
std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
    gmtime_s(&tm, &in_time_t);
    std::ostringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
    return ss.str();
}

// Utility function to escape JSON strings
std::string escapeJsonString(const std::string& input) {
    std::ostringstream ss;
    for (auto iter = input.cbegin(); iter != input.cend(); iter++) {
        switch (*iter) {
        case '\"': ss << "\\\""; break;
        case '\\': ss << "\\\\"; break;
        case '\b': ss << "\\b"; break;
        case '\f': ss << "\\f"; break;
        case '\n': ss << "\\n"; break;
        case '\r': ss << "\\r"; break;
        case '\t': ss << "\\t"; break;
        default: ss << *iter; break;
        }
    }
    return ss.str();
}

// DiscordEmbed implementation
DiscordEmbed::DiscordEmbed(const std::optional<std::string>& title, const std::optional<std::string>& description)
    : title(title), description(description) {}

void DiscordEmbed::setTitle(const std::string& title) {
    this->title = title;
}

void DiscordEmbed::setDescription(const std::string& description) {
    this->description = description;
}

void DiscordEmbed::setUrl(const std::string& url) {
    this->url = url;
}

void DiscordEmbed::setTimestamp(const std::optional<std::string>& timestamp) {
    if (timestamp) {
        this->timestamp = timestamp.value();
    }
    else {
        this->timestamp = getCurrentTimestamp();
    }
}

void DiscordEmbed::setColor(int color) {
    this->color = color;
}

void DiscordEmbed::setFooter(const std::string& text, const std::optional<std::string>& icon_url) {
    this->footer = { {"text", text}, {"icon_url", icon_url} };
}

void DiscordEmbed::setImage(const std::string& url, const std::optional<std::string>& proxy_url, const std::optional<int>& height, const std::optional<int>& width) {
    this->image = { {"url", url}, {"proxy_url", proxy_url}, {"height", height}, {"width", width} };
}

void DiscordEmbed::setThumbnail(const std::string& url, const std::optional<std::string>& proxy_url, const std::optional<int>& height, const std::optional<int>& width) {
    this->thumbnail = { {"url", url}, {"proxy_url", proxy_url}, {"height", height}, {"width", width} };
}

void DiscordEmbed::setVideo(const std::optional<std::string>& url, const std::optional<int>& height, const std::optional<int>& width) {
    this->video = { {"url", url}, {"height", height}, {"width", width} };
}

void DiscordEmbed::setProvider(const std::optional<std::string>& name, const std::optional<std::string>& url) {
    this->provider = { {"name", name}, {"url", url} };
}

void DiscordEmbed::setAuthor(const std::string& name, const std::optional<std::string>& url, const std::optional<std::string>& icon_url, const std::optional<std::string>& proxy_icon_url) {
    this->author = { {"name", name}, {"url", url}, {"icon_url", icon_url}, {"proxy_icon_url", proxy_icon_url} };
}

void DiscordEmbed::addField(const std::string& name, const std::string& value, bool inline_field) {
    this->fields.push_back({ {"name", name}, {"value", value}, {"inline", inline_field} });
}

void DiscordEmbed::removeField(size_t index) {
    if (index < this->fields.size()) {
        this->fields.erase(this->fields.begin() + index);
    }
}

std::string DiscordEmbed::toJson() const {
    std::ostringstream ss;
    ss << "{";
    if (title) ss << "\"title\":\"" << escapeJsonString(*title) << "\",";
    if (description) ss << "\"description\":\"" << escapeJsonString(*description) << "\",";
    if (url) ss << "\"url\":\"" << escapeJsonString(*url) << "\",";
    if (timestamp) ss << "\"timestamp\":\"" << escapeJsonString(*timestamp) << "\",";
    if (color) ss << "\"color\":" << *color << ",";
    if (footer) {
        ss << "\"footer\":{";
        for (const auto& [key, value] : *footer) {
            if (value) ss << "\"" << key << "\":\"" << escapeJsonString(*value) << "\",";
        }
        ss.seekp(-1, ss.cur); // Remove trailing comma
        ss << "},";
    }
    if (image) {
        ss << "\"image\":{";
        for (const auto& [key, value] : *image) {
            if (value) ss << "\"" << key << "\":\"" << escapeJsonString(std::get<std::string>(*value)) << "\",";
        }
        ss.seekp(-1, ss.cur); // Remove trailing comma
        ss << "},";
    }
    if (thumbnail) {
        ss << "\"thumbnail\":{";
        for (const auto& [key, value] : *thumbnail) {
            if (value) ss << "\"" << key << "\":\"" << escapeJsonString(std::get<std::string>(*value)) << "\",";
        }
        ss.seekp(-1, ss.cur); // Remove trailing comma
        ss << "},";
    }
    if (video) {
        ss << "\"video\":{";
        for (const auto& [key, value] : *video) {
            if (value) ss << "\"" << key << "\":\"" << escapeJsonString(std::get<std::string>(*value)) << "\",";
        }
        ss.seekp(-1, ss.cur); // Remove trailing comma
        ss << "},";
    }
    if (provider) {
        ss << "\"provider\":{";
        for (const auto& [key, value] : *provider) {
            if (value) ss << "\"" << key << "\":\"" << escapeJsonString(*value) << "\",";
        }
        ss.seekp(-1, ss.cur); // Remove trailing comma
        ss << "},";
    }
    if (author) {
        ss << "\"author\":{";
        for (const auto& [key, value] : *author) {
            if (value) ss << "\"" << key << "\":\"" << escapeJsonString(*value) << "\",";
        }
        ss.seekp(-1, ss.cur); // Remove trailing comma
        ss << "},";
    }
    if (!fields.empty()) {
        ss << "\"fields\":[";
        for (const auto& field : fields) {
            ss << "{";
            for (const auto& [key, value] : field) {
                if (value) ss << "\"" << key << "\":\"" << escapeJsonString(std::get<std::string>(*value)) << "\",";
            }
            ss.seekp(-1, ss.cur); // Remove trailing comma
            ss << "},";
        }
        ss.seekp(-1, ss.cur); // Remove trailing comma
        ss << "],";
    }
    ss.seekp(-1, ss.cur); // Remove trailing comma
    ss << "}";
    return ss.str();
}

// DiscordWebhook implementation
DiscordWebhook::DiscordWebhook(const std::string& url) : url(url) {}

void DiscordWebhook::addEmbed(const DiscordEmbed& embed) {
    this->embeds.push_back(embed);
}

void DiscordWebhook::removeEmbed(size_t index) {
    if (index < this->embeds.size()) {
        this->embeds.erase(this->embeds.begin() + index);
    }
}

void DiscordWebhook::clearEmbeds() {
    this->embeds.clear();
}

void DiscordWebhook::addFile(const std::string& filename, const std::vector<uint8_t>& file_content) {
    this->files[filename] = file_content;
}

void DiscordWebhook::removeFile(const std::string& filename) {
    this->files.erase(filename);
}

void DiscordWebhook::clearFiles() {
    this->files.clear();
}

void DiscordWebhook::setContent(const std::string& content) {
    this->content = content;
}

void DiscordWebhook::setAvatarUrl(const std::string& avatar_url) {
    this->avatar_url = avatar_url;
}

void DiscordWebhook::setUsername(const std::string& username) {
    this->username = username;
}

void DiscordWebhook::setTTS(bool tts) {
    this->tts = tts;
}

void DiscordWebhook::setAllowedMentions(const std::map<std::string, std::vector<std::string>>& allowed_mentions) {
    this->allowed_mentions = allowed_mentions;
}

void DiscordWebhook::setProxies(const std::map<std::string, std::string>& proxies) {
    this->proxies = proxies;
}

void DiscordWebhook::setTimeout(int timeout) {
    this->timeout = timeout;
}

void DiscordWebhook::setRateLimitRetry(bool rate_limit_retry) {
    this->rate_limit_retry = rate_limit_retry;
}

void DiscordWebhook::setThreadId(const std::string& thread_id) {
    this->thread_id = thread_id;
}

void DiscordWebhook::setThreadName(const std::string& thread_name) {
    this->thread_name = thread_name;
}

void DiscordWebhook::setWait(bool wait) {
    this->wait = wait;
}

std::string DiscordWebhook::toJson() const {
    std::ostringstream ss;
    ss << "{";
    if (content) ss << "\"content\":\"" << escapeJsonString(*content) << "\",";
    if (avatar_url) ss << "\"avatar_url\":\"" << escapeJsonString(*avatar_url) << "\",";
    if (username) ss << "\"username\":\"" << escapeJsonString(*username) << "\",";
    if (tts) ss << "\"tts\":" << (*tts ? "true" : "false") << ",";
    if (allowed_mentions) {
        ss << "\"allowed_mentions\":{";
        for (const auto& [key, value] : *allowed_mentions) {
            ss << "\"" << key << "\":[";
            for (const auto& item : value) {
                ss << "\"" << escapeJsonString(item) << "\",";
            }
            ss.seekp(-1, ss.cur); // Remove trailing comma
            ss << "],";
        }
        ss.seekp(-1, ss.cur); // Remove trailing comma
        ss << "},";
    }
    if (thread_id) ss << "\"thread_id\":\"" << escapeJsonString(*thread_id) << "\",";
    if (wait) ss << "\"wait\":" << (*wait ? "true" : "false") << ",";
    if (!embeds.empty()) {
        ss << "\"embeds\":[";
        for (const auto& embed : embeds) {
            ss << embed.toJson() << ",";
        }
        ss.seekp(-1, ss.cur); // Remove trailing comma
        ss << "],";
    }
    ss.seekp(-1, ss.cur); // Remove trailing comma
    ss << "}";
    return ss.str();
}

void DiscordWebhook::execute() {
    HINTERNET hSession = WinHttpOpen(L"A Discord Webhook Client/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) {
        std::cerr << "WinHttpOpen failed: " << GetLastError() << std::endl;
        return;
    }

    URL_COMPONENTS urlComp;
    memset(&urlComp, 0, sizeof(urlComp));
    urlComp.dwStructSize = sizeof(urlComp);

    wchar_t host[256];
    wchar_t path[256];
    urlComp.lpszHostName = host;
    urlComp.dwHostNameLength = sizeof(host) / sizeof(wchar_t);
    urlComp.lpszUrlPath = path;
    urlComp.dwUrlPathLength = sizeof(path) / sizeof(wchar_t);

    std::wstring wUrl = stringToWstring(url);

    if (!WinHttpCrackUrl(wUrl.c_str(), 0, 0, &urlComp)) {
        std::cerr << "WinHttpCrackUrl failed: " << GetLastError() << std::endl;
        WinHttpCloseHandle(hSession);
        return;
    }

    HINTERNET hConnect = WinHttpConnect(hSession, urlComp.lpszHostName, urlComp.nPort, 0);
    if (!hConnect) {
        std::cerr << "WinHttpConnect failed: " << GetLastError() << std::endl;
        WinHttpCloseHandle(hSession);
        return;
    }

    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST", urlComp.lpszUrlPath, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, urlComp.nScheme == INTERNET_SCHEME_HTTPS ? WINHTTP_FLAG_SECURE : 0);
    if (!hRequest) {
        std::cerr << "WinHttpOpenRequest failed: " << GetLastError() << std::endl;
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return;
    }

    std::string jsonPayload = toJson();
    if (!WinHttpSendRequest(hRequest, L"Content-Type: application/json\r\n", -1, (LPVOID)jsonPayload.c_str(), jsonPayload.size(), jsonPayload.size(), 0)) {
        std::cerr << "WinHttpSendRequest failed: " << GetLastError() << std::endl;
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return;
    }

    if (!WinHttpReceiveResponse(hRequest, NULL)) {
        std::cerr << "WinHttpReceiveResponse failed: " << GetLastError() << std::endl;
    }

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
}