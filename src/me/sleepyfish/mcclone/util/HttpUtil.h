//
// Created by SleepyFish on 17.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_HTTPUTIL_H
#define MCCLONE_HTTPUTIL_H

#include <string>
#include <filesystem>
#include <functional>
#include <unordered_map>
#include <future>
#include <vector>
#include <map>

#include <curl/curl.h>

/**
 * @author SleepyFish
 * @brief HTTP utility
 */
class HttpUtil {

public:

    static std::atomic<int> downloadThreadsStarted;

    static std::string buildPostString(const std::map<std::string, std::string> &data);

    static std::string postMap(const std::string &url, const std::map<std::string, std::string> &data, bool skipLoggingErrors = false);

    static std::string get(const std::string &url);

    static std::future<void> downloadResourcePack(
            const std::filesystem::path &saveFile,
            const std::string &packUrl,
            const std::unordered_map<std::string, std::string> &headers,
            size_t maxSize,
            IProgressUpdate *progressUpdate = nullptr
    );

    static int getSuitableLanPort();

    static std::string sha1File(const std::filesystem::path &path);
    static std::string sha1Bytes(const std::vector<uint8_t> &data);

private:

    struct DownloadContext {
        std::ofstream *file;
        size_t maxSize;
        size_t downloaded;
        float contentLength;
        IProgressUpdate *progressUpdate;
        std::atomic<bool> *cancelled;
    };

    static size_t writeCallback(void *ptr, size_t size, size_t nmemb, void *userdata);
    static int progressCallback(void *userdata, ::curl_off_t dltotal, ::curl_off_t dlnow, ::curl_off_t, ::curl_off_t);
    static std::string post(const std::string &url, const std::string &content, bool skipLoggingErrors);

};


#endif //MCCLONE_HTTPUTIL_H
