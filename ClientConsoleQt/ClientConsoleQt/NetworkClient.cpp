#include "NetworkClient.h"
#include <QEventLoop>
#include <QtNetwork/QNetworkRequest>
#include <QUrl>

NetworkClient::NetworkClient(const std::string& baseUrl, QObject* parent)
    : QObject(parent), m_baseUrl(baseUrl)
{
}

Response NetworkClient::Post(const std::string& endpoint, const nlohmann::json& payload) {
    std::string fullUrl = m_baseUrl + endpoint;
    QNetworkRequest request((QUrl(QString::fromStdString(fullUrl))));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    std::string jsonStr = payload.dump();
    QByteArray data(jsonStr.c_str(), jsonStr.length());

    QNetworkReply* reply = m_manager.post(request, data);
    return handleReply(reply);
}

Response NetworkClient::Get(const std::string& endpoint) {
    std::string fullUrl = m_baseUrl + endpoint;
    QNetworkRequest request((QUrl(QString::fromStdString(fullUrl))));

    QNetworkReply* reply = m_manager.get(request);
    return handleReply(reply);
}

Response NetworkClient::handleReply(QNetworkReply* reply) {
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    Response resp;
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (statusCode.isValid()) {
        resp.status_code = statusCode.toInt();
    }
    else {
        resp.status_code = 0;
    }

    if (reply->error() != QNetworkReply::NoError) {
        if (resp.status_code == 0) resp.status_code = 500;
    }

    resp.text = reply->readAll().toStdString();
    reply->deleteLater();
    
    return resp;
}
