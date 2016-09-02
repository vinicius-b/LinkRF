/*
 * Framming.h

 *
 *  Created on: 25 de ago de 2016
 *      Author: aluno
 */
#include "Serial.h"
#include <string.h>
#include <iostream>
#include <cstring>

#ifndef FRAMMING_H_
#define FRAMMING_H_

class Framework {

public:
	Framework(Serial& s):serial(s){};
	virtual ~Framework();
	void send(char *buffer, int len);
	void receive(char* buffer);
private:
	Serial& serial;
//	int min_bytes, max_bytes;
//	char *buffer;
//	int n_bytes;
//	int state;
//	bool handle(char byte, int position);
};

#endif /* FRAMMING_H_ */
