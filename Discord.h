#pragma once
#include <string>
#include <vector>
#include <map>
#include <optional>
#include <variant>

class DiscordEmbed {
public:
    DiscordEmbed(const std::optional<std::string>& title = std::nullopt,
        const std::optional<std::string>& description = std::nullopt);

    void setTitle(const std::string& title);
    void setDescription(const std::string& description);
    void setUrl(const std::string& url);
    void setTimestamp(const std::optional<std::string>& timestamp = std::nullopt);
    void setColor(int color);
    void setFooter(const std::string& text, const std::optional<std::string>& icon_url = std::nullopt);
    void setImage(const std::string& url, const std::optional<std::string>& proxy_url = std::nullopt,
        const std::optional<int>& height = std::nullopt, const std::optional<int>& width = std::nullopt);
    void setThumbnail(const std::string& url, const std::optional<std::string>& proxy_url = std::nullopt,
        const std::optional<int>& height = std::nullopt, const std::optional<int>& width = std::nullopt);
    void setVideo(const std::optional<std::string>& url = std::nullopt, const std::optional<int>& height = std::nullopt,
        const std::optional<int>& width = std::nullopt);
    void setProvider(const std::optional<std::string>& name = std::nullopt, const std::optional<std::string>& url = std::nullopt);
    void setAuthor(const std::string& name, const std::optional<std::string>& url = std::nullopt,
        const std::optional<std::string>& icon_url = std::nullopt, const std::optional<std::string>& proxy_icon_url = std::nullopt);
    void addField(const std::string& name, const std::string& value, bool inline_field = true);
    void removeField(size_t index);

    std::string toJson() const;

private:
    std::optional<std::string> title;
    std::optional<std::string> description;
    std::optional<std::string> url;
    std::optional<std::string> timestamp;
    std::optional<int> color;
    std::optional<std::map<std::string, std::optional<std::string>>> footer;
    std::optional<std::map<std::string, std::optional<std::variant<std::string, int>>>> image;
    std::optional<std::map<std::string, std::optional<std::variant<std::string, int>>>> thumbnail;
    std::optional<std::map<std::string, std::optional<std::variant<std::string, int>>>> video;
    std::optional<std::map<std::string, std::optional<std::string>>> provider;
    std::optional<std::map<std::string, std::optional<std::string>>> author;
    std::vector<std::map<std::string, std::optional<std::variant<std::string, bool>>>> fields;
};

class DiscordWebhook {
public:
    DiscordWebhook(const std::string& url);

    void addEmbed(const DiscordEmbed& embed);
    void removeEmbed(size_t index);
    void clearEmbeds();
    void addFile(const std::string& filename, const std::vector<uint8_t>& file_content);
    void removeFile(const std::string& filename);
    void clearFiles();
    void setContent(const std::string& content);
    void setAvatarUrl(const std::string& avatar_url);
    void setUsername(const std::string& username);
    void setTTS(bool tts);
    void setAllowedMentions(const std::map<std::string, std::vector<std::string>>& allowed_mentions);
    void setProxies(const std::map<std::string, std::string>& proxies);
    void setTimeout(int timeout);
    void setRateLimitRetry(bool rate_limit_retry);
    void setThreadId(const std::string& thread_id);
    void setThreadName(const std::string& thread_name);
    void setWait(bool wait);

    void execute();
    std::string toJson() const;

private:
    std::string url;
    std::optional<std::string> content;
    std::optional<std::string> avatar_url;
    std::optional<std::string> username;
    std::optional<bool> tts;
    std::optional<std::map<std::string, std::vector<std::string>>> allowed_mentions;
    std::optional<std::map<std::string, std::string>> proxies;
    std::optional<int> timeout;
    std::optional<bool> rate_limit_retry;
    std::optional<std::string> thread_id;
    std::optional<std::string> thread_name;
    std::optional<bool> wait;
    std::vector<DiscordEmbed> embeds;
    std::map<std::string, std::vector<uint8_t>> files;
};