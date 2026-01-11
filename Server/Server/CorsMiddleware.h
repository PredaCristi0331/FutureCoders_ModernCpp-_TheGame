#pragma once
#include <crow.h>
#include <string>

namespace http
{
    class CorsMiddleware
    {
    public:
        static void AddCorsHeaders(crow::response& response)
        {
            response.add_header("Access-Control-Allow-Origin", "*");
            response.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
            response.add_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
            response.add_header("Access-Control-Max-Age", "3600");
        }

        static crow::response HandlePreflight()
        {
            crow::response response(204);
            AddCorsHeaders(response);
            return response;
        }
    };
}
