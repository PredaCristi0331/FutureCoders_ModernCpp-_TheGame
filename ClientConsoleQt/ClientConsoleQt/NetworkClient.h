#pragma once

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QString>
#include <string>
#include <nlohmann/json.hpp>

struct Response {
    int status_code;
    std::string text;
};

class NetworkClient : public QObject {
    Q_OBJECT
public:
    explicit NetworkClient(const std::string& baseUrl, QObject* parent = nullptr);

    Response Post(const std::string& endpoint, const nlohmann::json& payload);
    Response Get(const std::string& endpoint);

private:
    std::string m_baseUrl;
    QNetworkAccessManager m_manager;

    Response handleReply(QNetworkReply* reply);
};
