/*
 * Framming.cpp
 *
 *  Created on: 25 de ago de 2016
 *      Author: aluno
 */

#include "Framework.h"

Framework::~Framework() {
}

/*
 * void Framming::send(char *buffer, int len);
 *
 * char *buffer - It is the data that must be placed in a frame.
 * int len - The size of the data in bytes
 *
 * This function delimitate the data with flags 0x7E, replace false flags
 * and send the frame through the serial port.
 */
void Framework::send(char *buffer, int len){

	char frame[2*BUFSIZE+2];
	
	std::cout << "Framework started" << std::endl;

	frame[0] = 0x7E;							// Initial delimitation of the frame using flag 0x7E (01111110)
	int i,j;
	for( i=0, j=1 ; i <= len ; i++,j++ ){
		switch(buffer[i]){
			case(0x7E):							// If there is a 0x7E byte in the data, it is placed 0x7D5E in frame
				frame[j] = 0x7D;
				frame[j+1] = 0x5E;
				j = j + 1;
				std::cout << "INFO: 0x7E founded in position "<< i << " of the data." << std::endl;
				break;
			case(0x7D):							// If there is a 0x7D byte in the data, it is placed 0x7D5D in frame
				frame[j] = 0x7D;
				frame[j+1] = 0x5D;
				j = j + 1;
				std::cout << "INFO: 0x7D founded in position "<< i << " of the data." << std::endl;
				break;
			default:
				frame[j] = buffer[i];			// Normal copy
		}
	}
	frame[j-1] = 0x7E;							// End delimitation of the frame using flag 0x7E (01111110)
	frame[j] = '\n';

	int n = serial.write(frame, strlen(frame));

	if(! n > 0){
		std::cout << "Error in writing a frame in the serial port" << n << std::endl;
	}
}

/*
 * char* Framming::receive(char* buffer);
 *
 * char* frame - It is the buffer where is placed the data
 *
 * This function receive the frame from serial port, extract
 * the date from the frame ignoring the flags and false flags
 * and put it in the buffer.
 *
 */
void Framework::receive(char* buffer){
	
	std::cout << "Removal of framework started" << std::endl;

	char frame[BUFSIZE];

	int n = serial.read(frame, BUFSIZE, false);

	if(! n > 0){
		std::cout << "Error in reading a frame from serial port" << n << std::endl;
	}else{
		std::cout << "Frame received from serial: " << frame << std::endl;
	}

	unsigned int start;							// Ignore any trash in the beginning of the frame
	for(start = 0; start <= strlen(frame); start++){
		if(frame[start] == 0x7E){
			break;
		}
	}

	unsigned int i,j;
	for(i = start+1, j = 0 ; i <= strlen(frame)-1 ; i++,j++ ){

		if(frame[i] == 0x7E){					// Founded end delimitation
			break;
		}

		if(frame[i] == 0x7D){					// Founded false flag
			switch(frame[i+1]){
				case(0x5E):
					buffer[j] = 0x7E;
					i = i+1;
					std::cout << "0x7D5E found in position "<< i << " of the frame" << std::endl;
					break;
				case(0x5D):
					buffer[j] = 0x7D;
					i = i+1;
					std::cout << "0x7D5D found in position "<< i << " of the frame" << std::endl;
					break;
				default:
					buffer[j] = frame[i]; 		// Normal copy
					break;
			}
		}else{
			buffer[j] = frame[i];				// Normal copy
		}
	}

	std::cout << "Data received: " << buffer  << std::endl;
}
