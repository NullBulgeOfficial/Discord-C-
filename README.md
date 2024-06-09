
```
 _                 _        _        ______            _        _______  _______  _______  _______  _______           _______ 
( (    /||\     /|( \      ( \      (  ___ \ |\     /|( \      (  ____ \(  ____ \(  ____ \(  ____ )(  ___  )|\     /|(  ____ )
|  \  ( || )   ( || (      | (      | (   ) )| )   ( || (      | (    \/| (    \/| (    \/| (    )|| (   ) || )   ( || (    )|
|   \ | || |   | || |      | |      | (__/ / | |   | || |      | |      | (__    | |      | (____)|| |   | || |   | || (____)|
| (\ \) || |   | || |      | |      |  __ (  | |   | || |      | | ____ |  __)   | | ____ |     __)| |   | || |   | ||  _____)
| | \   || |   | || |      | |      | (  \ \ | |   | || |      | | \_  )| (      | | \_  )| (\ (   | |   | || |   | || (      
| )  \  || (___) || (____/\| (____/\| )___) )| (___) || (____/\| (___) || (____/\| (___) || ) \ \__| (___) || (___) || )      
|/    )_)(_______)(_______/(_______/|/ \___/ (_______)(_______/(_______)(_______/(_______)|/   \__/(_______)(_______)|/       
                                                                                                                              
```

# Discord Webhook C++ Library

A simple C++ library for sending messages and embeds to a Discord channel using webhooks.

## Features

- Send messages to Discord via webhooks.
- Customize the message content, username, and avatar.
- Add rich embeds with various properties like title, description, color, timestamp, etc.
- Support for adding files to the webhook.
- Configure allowed mentions, TTS, and other webhook options.

## Requirements

- Windows OS

## Installation

1. Clone the repository:
    ```bash
    git clone https://github.com/NullBulgeOfficial/Discord-C-.git
    ```
2. Include the `Discord.h` in your project.

## Usage

Here's an example of how to use the library to send a message with an embed to a Discord channel:

```cpp
#include "Discord.h"

int main() {
    const std::string webhook_url = "https://discord.com/api/webhooks/YOUR_WEBHOOK_ID/YOUR_WEBHOOK_TOKEN";
    DiscordWebhook webhook(webhook_url);

    webhook.setContent("Hello from C++!");
    webhook.setUsername("Bot");
    webhook.setAvatarUrl("https://example.com/avatar.png");

    DiscordEmbed embed("Embed Title", "Embed Description");
    embed.setColor(0x00FF00);
    embed.setTimestamp();

    webhook.addEmbed(embed);

    webhook.execute();

    return 0;
}
```

## API Reference

### `DiscordWebhook`

#### Constructor
```cpp
DiscordWebhook(const std::string& url);
```

#### Methods
- `void setContent(const std::string& content);`
- `void setAvatarUrl(const std::string& avatar_url);`
- `void setUsername(const std::string& username);`
- `void setTTS(bool tts);`
- `void setAllowedMentions(const std::map<std::string, std::vector<std::string>>& allowed_mentions);`
- `void setProxies(const std::map<std::string, std::string>& proxies);`
- `void setTimeout(int timeout);`
- `void setRateLimitRetry(bool rate_limit_retry);`
- `void setThreadId(const std::string& thread_id);`
- `void setThreadName(const std::string& thread_name);`
- `void setWait(bool wait);`
- `void addEmbed(const DiscordEmbed& embed);`
- `void removeEmbed(size_t index);`
- `void clearEmbeds();`
- `void addFile(const std::string& filename, const std::vector<uint8_t>& file_content);`
- `void removeFile(const std::string& filename);`
- `void clearFiles();`
- `void execute();`

### `DiscordEmbed`

#### Constructor
```cpp
DiscordEmbed(const std::optional<std::string>& title, const std::optional<std::string>& description);
```

#### Methods
- `void setTitle(const std::string& title);`
- `void setDescription(const std::string& description);`
- `void setUrl(const std::string& url);`
- `void setTimestamp(const std::optional<std::string>& timestamp = std::nullopt);`
- `void setColor(int color);`
- `void setFooter(const std::string& text, const std::optional<std::string>& icon_url = std::nullopt);`
- `void setImage(const std::string& url, const std::optional<std::string>& proxy_url = std::nullopt, const std::optional<int>& height = std::nullopt, const std::optional<int>& width = std::nullopt);`
- `void setThumbnail(const std::string& url, const std::optional<std::string>& proxy_url = std::nullopt, const std::optional<int>& height = std::nullopt, const std::optional<int>& width = std::nullopt);`
- `void setVideo(const std::optional<std::string>& url = std::nullopt, const std::optional<int>& height = std::nullopt, const std::optional<int>& width = std::nullopt);`
- `void setProvider(const std::optional<std::string>& name = std::nullopt, const std::optional<std::string>& url = std::nullopt);`
- `void setAuthor(const std::string& name, const std::optional<std::string>& url = std::nullopt, const std::optional<std::string>& icon_url = std::nullopt, const std::optional<std::string>& proxy_icon_url = std::nullopt);`
- `void addField(const std::string& name, const std::string& value, bool inline_field);`
- `void removeField(size_t index);`
- `std::string toJson() const;`

## Contributing

Contributions are welcome! Please open an issue or submit a pull request.