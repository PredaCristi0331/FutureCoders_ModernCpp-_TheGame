#pragma once
#include <crow.h>
#include <string>

namespace http
{
    struct ValidationResult
    {
        bool isValid;
        crow::response errorResponse;

        ValidationResult() : isValid(true), errorResponse(200) {}
        ValidationResult(crow::response&& response) : isValid(false), errorResponse(std::move(response)) {}
        
        explicit operator bool() const { return !isValid; }
    };

    class RequestValidator
    {
    public:
        static ValidationResult ValidateJSON(const crow::request& req)
        {
            auto body = crow::json::load(req.body);
            if (!body)
            {
                return ValidationResult(CreateErrorResponse(400, "Invalid JSON format", "Request body must be valid JSON"));
            }
            return ValidationResult();
        }

        static ValidationResult ValidateRequiredField(
            const crow::json::rvalue& body,
            const std::string& fieldName)
        {
            if (!body.has(fieldName))
            {
                return ValidationResult(CreateErrorResponse(400, "Missing field", 
                    "Required field '" + fieldName + "' is missing"));
            }
            return ValidationResult();
        }

        static ValidationResult ValidateStringNotEmpty(
            const std::string& value,
            const std::string& fieldName)
        {
            if (value.empty())
            {
                return ValidationResult(CreateErrorResponse(400, "Invalid " + fieldName,
                    fieldName + " cannot be empty"));
            }
            return ValidationResult();
        }

        static ValidationResult ValidateStringLength(
            const std::string& value,
            const std::string& fieldName,
            size_t minLength,
            size_t maxLength = std::string::npos)
        {
            if (value.length() < minLength)
            {
                return ValidationResult(CreateErrorResponse(400, "Invalid " + fieldName,
                    fieldName + " must be at least " + std::to_string(minLength) + " characters long"));
            }

            if (maxLength != std::string::npos && value.length() > maxLength)
            {
                return ValidationResult(CreateErrorResponse(400, "Invalid " + fieldName,
                    fieldName + " must not exceed " + std::to_string(maxLength) + " characters"));
            }

            return ValidationResult();
        }

        static crow::response CreateErrorResponse(
            int code,
            const std::string& error,
            const std::string& message)
        {
            crow::json::wvalue response;
            response["error"] = error;
            response["message"] = message;
            return crow::response(code, response);
        }

        static crow::response CreateSuccessResponse(
            const std::string& status,
            const std::string& message)
        {
            crow::json::wvalue response;
            response["status"] = status;
            response["message"] = message;
            return crow::response(200, response);
        }
    };
}
