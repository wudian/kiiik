#pragma once

namespace network {

class TcpSession;

class TcpMessage
{
	friend class TcpSession;

public:
	enum {
		header_length = 6,
		body_length_value = 12,
		max_buf_length = 1024,
		max_body_length = 9999999
	};

	TcpMessage(TcpSession *session);
	void decode(char* buf, int len);

private:
	bool check_header(char* title, int len);

	char header_[header_length + body_length_value];
	char* buf_;
	int buf_len_;
	bool b_first_read_;
	int read_size_;

	TcpSession *session_;
};

}