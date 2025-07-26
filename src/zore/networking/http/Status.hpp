
namespace zore::net::http {

    enum class Status {
        // 1xx: Informational.
        CONTINUE                        = 100, // The request has been received and the client should continue with the request.
        SWITCHING_PROTOCOL              = 101, // The server is switching protocols as requested by the client.
        PROCESSING                      = 102, // The server has received the request and is processing it, but no response is available yet.
        EARLY_HINTS                     = 103, // The server is sending preliminary information before the final response.

        // 2xx: Success.
        OK                              = 200, // The request has succeeded.
		CREATED                         = 201, // The resource has successfully been created.
		ACCEPTED                        = 202, // The request has been accepted, but will be processed later by the server.
        NON_AUTHORITATIVE_INFORMATION   = 203, // The server is returning information from a third-party source.
		NO_CONTENT                      = 204, // The server didn't send any data in return.
		RESET_CONTENT                   = 205, // The server informs the client that it should clear the view (form) that caused the request to be sent.
		PARTIAL_CONTENT                 = 206, // The server has sent a part of the resource, as a response to a partial GET request.
        MULTI_STATUS                    = 207, // The server is returning multiple status codes for different parts of the request.
        ALREADY_REPORTED                = 208, // The server has already reported the status of the requested resource.
        IM_USED                         = 226, // The server has fulfilled a request for the resource, but the response is a representation of the result of one or more instance-manipulations applied to the current instance.

		// 3xx: Redirection.
		MULTIPLE_CHOICES                = 300, // The requested page can be accessed from several locations.
		MOVED_PERMANENTLY               = 301, // The requested page has permanently moved to a new location.
		MOVED_TEMPORARILY               = 302, // The requested page has temporarily moved to a new location.
        SEE_OTHER                       = 303, // The requested page can be found at a different location, and the client should use a GET request to access it.
		NOT_MODIFIED                    = 304, // For conditional requests, means the requested page hasn't changed and doesn't need to be refreshed.
        USE_PROXY                       = 305, // The requested page must be accessed through a proxy.
        TEMPORARY_REDIRECT              = 307, // The requested page has temporarily moved to a new location, but the client should use the same method for the next request.
        PERMANENT_REDIRECT              = 308, // The requested page has permanently moved to a new location, and the client should use the same method for the next request.

		// 4xx: Client Error.
		BAD_REQUEST                     = 400, // The server cannot or will not process the request (client error).
		UNAUTHORIZED                    = 401, // The requested page needs an authentication to be accessed.
        PAYMENT_REQUIRED                = 402, // The requested page requires payment to be accessed.
		FORBIDDEN                       = 403, // The requested page cannot be accessed at all, even with authentication.
		NOT_FOUND                       = 404, // The requested page doesn't exist.
        METHOD_NOT_ALLOWED              = 405, // The requested method is not allowed for the requested page.
        NOT_ACCEPTABLE                  = 406, // The requested page cannot be provided in the requested format.
		PROXY_AUTHENTICATION_REQUIRED   = 407, // The requested content requires authentication by a proxy.
        REQUEST_TIMEOUT                 = 408, // The server timed out waiting for the request.
        CONFLICT                        = 409, // The request conflicts with the current state of the server.
        GONE                            = 410, // The requested page is no longer available.
		LENGTH_REQUIRED                 = 411, // The server requires a "Content-Length" header field in the request.
		PRECONDITION_FAILED             = 412, // The server doesn't meet one of the preconditions specified in the request.
		CONTENT_TOO_LARGE               = 413, // The request payload is too large.
		URI_TOO_LONG                    = 414, // The requested URI is too long for the server to process.
		UNSUPPORTED_MEDIA_TYPE          = 415, // The requested page cannot be provided in the requested media type.
		RANGE_NOT_SATISFIABLE           = 416, // The requested range cannot be satisfied.
		EXPECTATION_FAILED              = 417, // The server cannot meet the requirements of the "Expect" request header field.
		IM_A_TEAPOT                     = 418, // The server refuses to brew coffee because it is a teapot (RFC 7168).
		MISDIRECTED_REQUEST             = 421, // The server is not able to produce a response for the request, due to a misdirected request.
		UNPROCESSABLE_CONTENT           = 422, // The server understands the request, but it cannot process it due to semantic errors.
		LOCKED                          = 423, // The requested resource is locked and cannot be modified.
		FAILED_DEPENDENCY               = 424, // The request failed due to a failure of a previous request that the current request depends on.
		TOO_EARLY                       = 425, // The server is unwilling to risk processing a request that might be replayed.
		UPGRADE_REQUIRED                = 426, // The client should switch to a different protocol, as requested by the server.
		PRECONDITION_REQUIRED           = 428, // The server requires the request to be conditional.
		TOO_MANY_REQUESTS               = 429, // The client has sent too many requests in a given amount of time.
		REQUEST_HEADER_FIELDS_TOO_LARGE = 431, // The server is unwilling to process the request because the request header fields are too large.
		UNAVAILABLE_FOR_LEGAL_REASONS   = 451, // The requested resource is unavailable for legal reasons (e.g., copyright infringement, government censorship).

		// 5xx: Server Error.
		INTERNAL_SERVER_ERROR           = 500, // The server encountered an unexpected error.
		NOT_IMPLEMENTED                 = 501, // The server doesn't implement a requested feature.
		BAD_GATEWAY                     = 502, // The gateway server has received an error from the source server.
		SERVICE_UNAVAILABLE             = 503, // The server is temporarily unavailable (overloaded, in maintenance, ...).
		GATEWAY_TIMEOUT                 = 504, // The gateway server couldn't receive a response from the source server.
		HTTP_VERSION_NOT_SUPPORTED      = 505, // The server doesn't support the requested HTTP version.
		VARIANT_ALSO_NEGOTIATES         = 506, // The server has an internal configuration error that prevents it from fulfilling the request.
		INSUFFICIENT_STORAGE            = 507, // The server is unable to store the representation needed to complete the request.
		LOOP_DETECTED                   = 508, // The server detected an infinite loop while processing the request.
		NOT_EXTENDED                    = 510, // The server requires further extensions to fulfill the request.
		NETWORK_AUTHENTICATION_REQUIRED = 511, // The client needs to authenticate to gain network access.

		// 10xx: Custom codes.
		INVALID_RESPONSE                = 1000 // Connection with server failed.
    };
}