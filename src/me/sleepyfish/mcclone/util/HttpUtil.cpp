//
// Created by SleepyFish on 17.09.2026.
// Project: mcclone
//

#include "HttpUtil.h"

#include "IProgressUpdate.h"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <iostream>
#include <regex>

#include <openssl/sha.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif //_WIN32

std::atomic<int> HttpUtil::downloadThreadsStarted{0};

static std::string urlEncode(const std::string &value) {
    ::CURL *curl = ::curl_easy_init();
    if (!curl) return value;

    char *encoded = ::curl_easy_escape(curl, value.c_str(), static_cast<int>(value.length()));
    std::string result = encoded ? encoded : value;
    ::curl_free(encoded);
    ::curl_easy_cleanup(curl);
    return result;
}

size_t HttpUtil::writeCallback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    auto *ctx = static_cast<DownloadContext*>(userdata);
    size_t total = size * nmemb;

    if (ctx->maxSize > 0 && ctx->downloaded + total > ctx->maxSize) {
        std::cerr << "HttpUtil: filesize was bigger than maximum allowed (got >= "
                  << ctx->downloaded + total << ", limit was " << ctx->maxSize << ")\n";
        return 0; // abort
    }

    if (ctx->cancelled && ctx->cancelled->load()) {
        return 0; // abort
    }

    ctx->file->write(static_cast<char*>(ptr), total);
    ctx->downloaded += total;

    if (ctx->progressUpdate && ctx->contentLength > 0) {
        ctx->progressUpdate->setLoadingProgress(
                static_cast<int>(static_cast<float>(ctx->downloaded) / ctx->contentLength * 100.0f)
        );
    }

    return total;
}

int HttpUtil::progressCallback(void *userdata, ::curl_off_t dltotal, ::curl_off_t dlnow, ::curl_off_t, ::curl_off_t) {
    auto *ctx = static_cast<DownloadContext*>(userdata);

    if (ctx->cancelled && ctx->cancelled->load()) {
        return 1; // abort
    }

    return 0;
}

std::string HttpUtil::buildPostString(const std::map<std::string, std::string> &data) {
    std::ostringstream sb;
    bool first = true;

    for (const auto &[key, value] : data) {
        if (!first) {
            sb << '&';
        }

        sb << urlEncode(key);

        if (!value.empty()) {
            sb << '=' << urlEncode(value);
        }

        first = false;
    }

    return sb.str();
}

std::string HttpUtil::post(const std::string &url, const std::string &content, bool skipLoggingErrors) {
    ::CURL *curl = ::curl_easy_init();
    if (!curl) {
        if (!skipLoggingErrors) {
            std::cerr << "HttpUtil: could not post to " << url << ": failed to init curl\n";
        }
        return "";
    }

    std::string response;

    auto responseWriter = [](void *ptr, size_t size, size_t nmemb, void *userdata) -> size_t {
        auto *resp = static_cast<std::string*>(userdata);
        resp->append(static_cast<char*>(ptr), size * nmemb);
        return size * nmemb;
    };

    struct ::curl_slist *headers = nullptr;
    headers = ::curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
    headers = ::curl_slist_append(headers, "Content-Language: en-US");

    std::string contentLength = "Content-Length: " + std::to_string(content.size());
    headers = ::curl_slist_append(headers, contentLength.c_str());

    ::curl_easy_setopt(curl, CURLOPT_URL,           url.c_str());
    ::curl_easy_setopt(curl, CURLOPT_POST,          1L);
    ::curl_easy_setopt(curl, CURLOPT_POSTFIELDS,    content.c_str());
    ::curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, static_cast<long>(content.size()));
    ::curl_easy_setopt(curl, CURLOPT_HTTPHEADER,    headers);
    ::curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, +responseWriter);
    ::curl_easy_setopt(curl, CURLOPT_WRITEDATA,     &response);
    ::curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    ::CURLcode res = ::curl_easy_perform(curl);

    ::curl_slist_free_all(headers);
    ::curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        if (!skipLoggingErrors) {
            std::cerr << "HttpUtil: could not post to " << url << ": " << ::curl_easy_strerror(res) << "\n";
        }
        return "";
    }

    return response;
}

std::string HttpUtil::postMap(const std::string &url, const std::map<std::string, std::string> &data, bool skipLoggingErrors) {
    return post(url, buildPostString(data), skipLoggingErrors);
}

std::string HttpUtil::get(const std::string &url) {
    ::CURL *curl = ::curl_easy_init();
    if (!curl) {
        throw std::runtime_error("HttpUtil: failed to initialize curl");
    }

    std::string response;

    auto responseWriter = [](void *ptr, size_t size, size_t nmemb, void *userdata) -> size_t {
        auto *resp = static_cast<std::string*>(userdata);
        resp->append(static_cast<char*>(ptr), size * nmemb);
        return size * nmemb;
    };

    ::curl_easy_setopt(curl, CURLOPT_URL,            url.c_str());
    ::curl_easy_setopt(curl, CURLOPT_HTTPGET,        1L);
    ::curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,  +responseWriter);
    ::curl_easy_setopt(curl, CURLOPT_WRITEDATA,      &response);
    ::curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    ::CURLcode res = ::curl_easy_perform(curl);
    ::curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        throw std::runtime_error(std::string("HttpUtil: GET failed: ") + ::curl_easy_strerror(res));
    }

    return response;
}

std::future<void> HttpUtil::downloadResourcePack(
        const std::filesystem::path &saveFile,
        const std::string &packUrl,
        const std::unordered_map<std::string, std::string> &headers,
        size_t maxSize,
        IProgressUpdate *progressUpdate
) {
    downloadThreadsStarted.fetch_add(1);

    return std::async(std::launch::async, [=]() mutable {
        if (progressUpdate) {
            progressUpdate->resetProgressAndMessage("Downloading Resource Pack");
            progressUpdate->displayLoadingString("Making Request...");
        }

        std::filesystem::create_directories(saveFile.parent_path());
        std::ofstream outFile(saveFile, std::ios::binary | std::ios::trunc);

        if (!outFile.is_open()) {
            throw std::ios_base::failure("HttpUtil: could not open file for writing: " + saveFile.string());
        }

        std::atomic<bool> cancelled{false};

        DownloadContext ctx{
                .file           = &outFile,
                .maxSize        = maxSize,
                .downloaded     = 0,
                .contentLength  = 0.0f,
                .progressUpdate = progressUpdate,
                .cancelled      = &cancelled,
        };

        ::CURL *curl = ::curl_easy_init();
        if (!curl) {
            throw std::runtime_error("HttpUtil: failed to initialize curl");
        }

        // apply request headers - mirrors the for loop over p_180192_2_
        ::curl_slist *headerList = nullptr;
        float headerProgress = 0.0f;
        float headerTotal    = static_cast<float>(headers.size());

        for (const auto &[key, value] : headers) {
            std::string header = key + ": " + value;
            headerList = ::curl_slist_append(headerList, header.c_str());

            if (progressUpdate && headerTotal > 0) {
                progressUpdate->setLoadingProgress(
                        static_cast<int>(++headerProgress / headerTotal * 100.0f)
                );
            }
        }

        ::curl_easy_setopt(curl, CURLOPT_URL,              packUrl.c_str());
        ::curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,    writeCallback);
        ::curl_easy_setopt(curl, CURLOPT_WRITEDATA,        &ctx);
        ::curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progressCallback);
        ::curl_easy_setopt(curl, CURLOPT_XFERINFODATA,     &ctx);
        ::curl_easy_setopt(curl, CURLOPT_NOPROGRESS,       0L);
        ::curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,   1L);
#ifdef MCCLONE_USER_AGENT
        ::curl_easy_setopt(curl, CURLOPT_USERAGENT,        std::string(MCCLONE_USER_AGENT).c_str());
#endif //MCCLONE_USER_AGENT
        ::curl_easy_setopt(curl, CURLOPT_HTTPHEADER,       headerList);

        // get content length for progress display - mirrors httpurlconnection.getContentLength()
        ::curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
        ::curl_easy_perform(curl);
        double contentLength = 0.0;
        ::curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &contentLength);
        ::curl_easy_setopt(curl, CURLOPT_NOBODY, 0L);
        ctx.contentLength = static_cast<float>(contentLength);

        if (progressUpdate && contentLength > 0) {
            progressUpdate->displayLoadingString(
                    "Downloading file (" + std::to_string(contentLength / 1000.0f / 1000.0f).substr(0, 4) + " MB)..."
            );
        }

        // mirrors: if saveFile exists and length matches contentLength, skip download
        if (std::filesystem::exists(saveFile) && contentLength > 0) {
            auto existingSize = static_cast<double>(std::filesystem::file_size(saveFile));
            if (existingSize == contentLength) {
                if (progressUpdate) progressUpdate->setDoneWorking();
                ::curl_slist_free_all(headerList);
                ::curl_easy_cleanup(curl);
                return;
            }
            std::cerr << "HttpUtil: deleting " << saveFile
                      << " as it does not match what we currently have ("
                      << contentLength << " vs our " << existingSize << ")\n";
            std::filesystem::remove(saveFile);
        }

        // check content length against max before downloading
        if (maxSize > 0 && contentLength > static_cast<double>(maxSize)) {
            if (progressUpdate) progressUpdate->setDoneWorking();
            ::curl_slist_free_all(headerList);
            ::curl_easy_cleanup(curl);
            throw std::ios_base::failure(
                    "HttpUtil: filesize is bigger than maximum allowed (file is "
                    + std::to_string(contentLength) + ", limit is " + std::to_string(maxSize) + ")"
            );
        }

        ::CURLcode res = ::curl_easy_perform(curl);

        ::curl_slist_free_all(headerList);
        ::curl_easy_cleanup(curl);
        outFile.close();

        if (progressUpdate) progressUpdate->setDoneWorking();

        if (res != CURLE_OK && res != CURLE_ABORTED_BY_CALLBACK) {
            std::filesystem::remove(saveFile);
            throw std::runtime_error(std::string("HttpUtil: download failed: ") + ::curl_easy_strerror(res));
        }
    });
}

int HttpUtil::getSuitableLanPort() {
#ifdef _WIN32
    ::WSADATA wsaData;
    ::WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif //_WIN32

    int sock = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        throw std::runtime_error("HttpUtil: could not create socket for LAN port");
    }

    ::sockaddr_in addr{};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = 0; // OS picks a free port

    if (::bind(sock, reinterpret_cast<::sockaddr*>(&addr), sizeof(addr)) < 0) {
#ifdef _WIN32
        ::closesocket(sock);
#else
        close(sock);
#endif //_WIN32
        throw std::runtime_error("HttpUtil: could not bind socket for LAN port");
    }

    ::socklen_t len = sizeof(addr);
    ::getsockname(sock, reinterpret_cast<::sockaddr*>(&addr), &len);
    int port = ntohs(addr.sin_port);

#ifdef _WIN32
    ::closesocket(sock);
    ::WSACleanup();
#else
    ::close(sock);
#endif //_WIN32

    return port;
}

std::string HttpUtil::sha1File(const std::filesystem::path &path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw std::ios_base::failure("HttpUtil: cannot open file for hashing: " + path.string());
    }

    ::SHA_CTX ctx;
    ::SHA1_Init(&ctx);

    char buffer[8192];
    while (file.read(buffer, sizeof(buffer))) {
        ::SHA1_Update(&ctx, buffer, static_cast<size_t>(file.gcount()));
    }
    if (file.gcount() > 0) {
        ::SHA1_Update(&ctx, buffer, static_cast<size_t>(file.gcount()));
    }

    unsigned char hash[SHA_DIGEST_LENGTH];
    ::SHA1_Final(hash, &ctx);

    std::ostringstream oss;
    for (unsigned char byte : hash) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }

    return oss.str();
}

std::string HttpUtil::sha1Bytes(const std::vector<uint8_t> &data) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    ::SHA1(data.data(), data.size(), hash);

    std::ostringstream oss;
    for (unsigned char byte : hash) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }

    return oss.str();
}
