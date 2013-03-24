#ifndef __RAVEN_CPP_H__
#define __RAVEN_CPP_H__

#include <cstdio>
#include <cstdlib>
#include <string>
#include <ostream>
#include <boost/property_tree/ptree.hpp>

namespace raven {

	/**
	 * @brief How to use:
	 *
	 *        raven::Message msg;
	 *        msg.put("message", "something");
	 *        msg.put("level", "error");
	 *        msg.put("extra.color", "red");
	 *        msg.put("extra.dim.x", "100");
	 *        msg.put("extra.dim.y", "200");
	 *        raven::capture(msg);
	 *
	 *        but recommended is macro:
	 *
	 *        raven::Message msg;
	 *        msg.put("message", "something");
	 *        msg.put("extra.color", "red");
	 *        msg.put("extra.dim.x", "100");
	 *        msg.put("extra.dim.y", "200");
	 *        raven_error(msg);
	 *
	 *        Sentry documentation:
	 *        http://sentry.readthedocs.org/en/latest/developer/client/index.html
	 */
	typedef boost::property_tree::ptree Message;

	/** 
	 * @brief Init module
	 *
	 *        Url must be like that:
	 *        "udp://<key>:<secret>@<host>:<port>/<project>"
	 *
	 *        If proc == true:
	 *
	 *          File "/proc/self/status" 
	 *          Add values for: VmPeak, VmSize, VmLck, VmHWM, VmRSS, 
	 *          VmData, VmStk, VmExe, VmLib, VmPTE, FDSize and Threads.
	 *
	 *          File "/proc/loadavg" - gets whole line.
	 * 
	 * @param url   url
	 * @param proc  attach information from /proc/
	 * 
	 * @return returns false if url is invalid or cannot open socket 
	 */
	bool init(const std::string& url, const bool proc = false);

	/** 
	 * @brief Send message
	 * 
	 * @param message  message to send
	 */
	void capture(Message& message);

	/** 
	 * @brief Get json string
	 * 
	 * @param message  your message
	 * @param output   string output
	 */
	void to_json_string(const Message& message, std::string& output);

	/** 
	 * @brief Get json stream
	 * 
	 * @param message  your message
	 * @param output   stream output
	 */
	void to_json_stream(const Message& message, std::ostream& output);

	/** 
	 * @brief Encode message to buffer (zlib + base64)
	 * 
	 * @param message  your message
	 * @param encoded  result string
	 */
	void encode(const Message& message, std::string& encoded);

	/** 
	 * @brief Decode sent message (base64 + zlib) 
	 * 
	 * @param encoded  encoded message
	 * @param message  decoded message
	 * 
	 * @return false if cannot decode
	 */
	bool decode(const std::string& encoded, Message& message);
};

/**
 * @brief Macros raven_{debug,info,warning,error,fatal}.
 */

#define raven_debug(message) do {                          \
	(message).put("level", "debug");                         \
	raven_capture((message));                                \
} while (0)

#define raven_info(message) do {                           \
	(message).put("level", "info");                          \
	raven_capture((message));                                \
} while (0)

#define raven_warning(message) do {                        \
	(message).put("level", "warning");                       \
	raven_capture((message));                                \
} while (0)

#define raven_error(message) do {                          \
	(message).put("level", "error");                         \
	raven_capture((message));                                \
} while (0)

#define raven_fatal(message) do {                          \
	(message).put("level", "fatal");                         \
	raven_capture((message));                                \
} while (0)

#define raven_capture(message) do {                        \
  char source_file[64];                                    \
  snprintf(source_file, sizeof(source_file), "%s:%d",      \
      __FILE__, __LINE__);                                 \
	(message).put("culprit", __func__);                      \
	(message).put("tags.source_file", source_file);          \
	(message).put("extra.source.file", source_file);         \
	(message).put("extra.source.func", __PRETTY_FUNCTION__); \
	raven::capture((message));                               \
} while (0)

#endif