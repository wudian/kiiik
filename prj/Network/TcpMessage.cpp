#include "Network/TcpMessage.h"
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "Network/TcpSession.h"

namespace network {

const char *CHECK_CODE = "Kiiik";

TcpMessage::TcpMessage(TcpSession *session) 
	: buf_(NULL), b_first_read_(true), read_size_(0), session_(session)
{
	memset(header_, 0, sizeof(header_));
}

void TcpMessage::decode(char* buf, int len)
{
	if (b_first_read_)
	{
		char* tmp_buf = buf;
		int tmp_len = len;
		int title_size = header_length + body_length_value;
		if (read_size_ > 0)	//之前有数据头没读完
		{
			tmp_len = len + read_size_;
			tmp_buf = new char[tmp_len];
			memcpy(tmp_buf, header_, read_size_);
			memcpy(tmp_buf + read_size_, buf, len);
			memset(header_, 0, sizeof(header_));
			read_size_ = 0;
		}

		if (!check_header(tmp_buf, tmp_len))
		{
			read_size_ = 0;
			memset(header_, 0, sizeof(header_));
			return;
		}

		if (tmp_len <= title_size)	//数据头未读完，不进入读取内容
		{
			read_size_ = tmp_len;
			memcpy(header_, tmp_buf, tmp_len);
			return;
		}

		//解析内容
		if (tmp_len == title_size + buf_len_)
		{
			if (NULL != session_->read_spi_ && session_->is_connect()) { 
				session_->read_spi_->OnReceive(session_, tmp_buf + title_size, buf_len_); 
			}
			return;
		}
		else if (tmp_len > title_size + buf_len_)
		{
			if (NULL != session_->read_spi_ && session_->is_connect()) {
				session_->read_spi_->OnReceive(session_, tmp_buf + title_size, buf_len_); 
			}
			return decode(tmp_buf + title_size + buf_len_, tmp_len - title_size - buf_len_);	//嵌套
		}
		else	//内容不能一次新读完
		{
			b_first_read_ = false;	//下次进入读内容
			read_size_ = tmp_len - title_size;		//这里的read_size_表示已读取的内容长度
			buf_ = new char[buf_len_];
			memcpy(buf_, tmp_buf + title_size, tmp_len - title_size);
			return;
		}	
	}

	//读取内容
	if (buf_len_ > read_size_ + len)	//内容仍未读完
	{
		memcpy(buf_ + read_size_, buf, len);
		read_size_ += len;
		return;
	}

	int rese_len = read_size_ + len - buf_len_;
	memcpy(buf_ + read_size_, buf, buf_len_ - read_size_);
	if (NULL != session_->read_spi_ && session_->is_connect()) { 
		session_->read_spi_->OnReceive(session_, buf_, buf_len_);
	}
	read_size_ = 0;
	b_first_read_ = true;
	delete [] buf_;
	buf_ = NULL;

	if (rese_len > 0)	//多出来的内容
	{
		return decode(buf + len - rese_len, rese_len);	//嵌套
	}
}

bool TcpMessage::check_header(char* title, int len) {
	if (memcmp(title, CHECK_CODE, (len<header_length?len:header_length)) != 0)
		return false;
	else if (len >= header_length+body_length_value) {
		char size_val[body_length_value + 1] = {0};
		memcpy(size_val, title + header_length, body_length_value);
		buf_len_ = atoi(size_val);
		if (buf_len_ <= 0 || buf_len_ > max_body_length)
			return false;
	}
	return true;
}

}