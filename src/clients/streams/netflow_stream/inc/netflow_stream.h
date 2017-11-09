#ifndef _NETFLOW_STREAM_H
#define _NETFLOW_STREAM_H

#include "stinger_net/proto/stinger-batch.pb.h"
#include "stinger_net/send_rcv.h"
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>

#undef LOG_AT_F
#undef LOG_AT_E
#undef LOG_AT_W
#undef LOG_AT_I
#undef LOG_AT_V
#undef LOG_AT_D

#define LOG_AT_D
#include "stinger_core/stinger_error.h"

using namespace gt::stinger;


int64_t get_current_timestamp(void);

struct NetflowParse {

	std::string src, dest, protocol, src_port, dest_port;
	int64_t time;

	// Order: src_ip dest_ip protocol time
	// Time is in format YYYYMMDDHHmmSS
	void parseLine(char *line) {
		// Split netflow line into tokens, space-delimited
		std::string str(line);
		std::istringstream iss(str);
		std::vector<std::string> tokens;
		std::copy(std::istream_iterator<std::string>(iss),
			 std::istream_iterator<std::string>(),
			 std::back_inserter<std::vector<std::string> >(tokens));

		this->src = tokens[0];
		this->dest = tokens[1];
		this->protocol = tokens[2];
		this->time = std::atol(tokens[3].c_str());

	}

};
double dpc_tic(void);
double dpc_toc(double);

#endif /* _NETFLOW_STREAM_H */
