/*
 * File: test.cpp
 * Edited: 01 Dec 2015
 * Author: Matthew Bauer
 */

#include <iostream>
#include "cy_argp.h"

using namespace cy;

int main(int argc, char **argv)
{
	cy::argp_register_single('a', ARGP_TYPE_NONE);
	cy::argp_register_single('s', ARGP_TYPE_STRING);
	cy::argp_register_single('i', ARGP_TYPE_INTEGER);
	cy::argp_register_single('d', ARGP_TYPE_DOUBLE);
	cy::argp_register_multi("a", ARGP_TYPE_NONE);
	cy::argp_register_multi("s", ARGP_TYPE_STRING);
	cy::argp_register_multi("i", ARGP_TYPE_INTEGER);
	cy::argp_register_multi("d", ARGP_TYPE_DOUBLE);
	if(!argp_begin(argc, (const char**)argv))
	{
		std::cerr << "bad" << std::endl;
		return -1;
	}
	cy::argp_arg apa;
	while(argp_next(apa))
	{
		if(apa.cmd_is_single)
		{
			char c = apa.cmd.single;
			if(c == 'a')
			{
				std::cout << 'a';
			}
			if(c == 's')
			{
				std::cout << apa.param.p_string;
			}
			if(c == 'i')
			{
				std::cout << apa.param.p_integer;
			}
			if(c == 'd')
			{
				std::cout << apa.param.p_double;
			}
		}
		else
		{
			char c = apa.cmd.multi.at(0);
			if(c == 'a')
			{
				std::cout << 'a';
			}
			if(c == 's')
			{
				std::cout << apa.param.p_string;
			}
			if(c == 'i')
			{
				std::cout << apa.param.p_integer;
			}
			if(c == 'd')
			{
				std::cout << apa.param.p_double;
			}
		}
	}
	argp_end();
	std::cout << std::endl;
	return 0;
}
