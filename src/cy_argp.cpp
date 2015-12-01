/*
 * File: cy_argp.cpp
 * Edited: 01 Dec 2015
 * Author: Matthew Bauer
 */

#include "cy_argp.h"
#include <map>
#include <vector>
#include <sstream>

std::map<char, cy::argp_type> g_singles;
std::map<std::string, cy::argp_type> g_multis;
void cy::argp_register_single(char cmd, cy::argp_type type)
{
	g_singles.insert(std::pair<char, cy::argp_type>(cmd, type));
}
void cy::argp_register_multi(const std::string& cmd, cy::argp_type type)
{
	g_multis.insert(std::pair<std::string, cy::argp_type>(cmd, type));
}
void cy::argp_clear_registry()
{
	g_singles.clear();
	g_multis.clear();
}

struct token
{
	bool is_cmd;
	bool is_single;
	std::string str;
};
std::vector<token> g_tokens;
std::vector<cy::argp_arg> g_processed_args;
int g_current_arg = 0; // placeholder for g_processed args
bool process_next_token(int& i)
{
	cy::argp_arg a; // next argp_arg to put in g_processed_args
	token t = g_tokens.at(i);
	i++;
	if(!t.is_cmd)
		return false; // no params allowed without commands first
	// check that command is registered
	cy::argp_type type;
	if(t.is_single)
	{
		// check singles map
		try
		{
			type = g_singles.at(t.str.at(0));
		}
		catch(...) // out of range probably
		{
			return false;
		}
		a.cmd_is_single = true;
		a.cmd.single = t.str.at(0);
	}
	else
	{
		// check multis map
		try
		{
			type = g_multis.at(t.str);
		}
		catch(...)
		{
			return false;
		}
		a.cmd_is_single = false;
		a.cmd.multi = t.str;
	}
	a.param_type = type;
	// check if the param is given the right parameters
	if(type == cy::argp_type::ARGP_TYPE_NONE)
	{
		// good to go
		g_processed_args.push_back(a);
		return true;
	}
	// else, process the parameter
	if(i >= g_tokens.size())
	{
		// no parameter given; error
		return false;
	}
	token p = g_tokens.at(i);
	if(p.is_cmd)
	{
		// uh oh, not a parameter! error
		return false;
	}
	if(type == cy::argp_type::ARGP_TYPE_STRING)
	{
		// good to go
		a.param.p_string = p.str;
		g_processed_args.push_back(a);
		i++; // increment counter
		return true;
	}
	if(type == cy::argp_type::ARGP_TYPE_INTEGER)
	{
		// check that it is actually an integer (by trying to convert), also check for . in string
		if(p.str.find_first_of('.') != std::string::npos)
		{ // . found, not an int
			return false;
		}
		if(!((std::stringstream(p.str)) >> a.param.p_integer))
		{
			// not an int; error
			return false;
		}
		// good to go
		g_processed_args.push_back(a);
		i++;
		return true;
	}
	if(type == cy::argp_type::ARGP_TYPE_DOUBLE)
	{
		// check that it actually is a double (by trying to convert)
		if(!((std::stringstream(p.str)) >> a.param.p_double))
		{
			// not a double; error
			return false;
		}
		// good to go
		g_processed_args.push_back(a);
		i++;
		return true;
	}
	// no idea how it would get here, but eh
	return false;
}
bool process()
{
	for(int i = 0; i < g_tokens.size(); /*no increment*/)
	{
		if(!process_next_token(i))
			return false;
	}
	return true;
}
bool add_to_token_list(const std::string& arg)
{
	if(arg.empty() || arg == "--") // only return false cases
		return false;
	
	token t; // token to add
	if(arg.size() == 1) // must be parameter
	{
		t.is_cmd = false;
		t.str = arg;
		g_tokens.push_back(t);
		return true;
	}
	//else
	if(arg.size() == 2) // must be parameter or single
	{
		if(arg.at(0) == '-')
		{// single
			t.is_cmd = true;
			t.is_single = true;
			t.str = arg.at(1);
			g_tokens.push_back(t);
			return true;
		}
		// (else) parameter
		t.is_cmd = false;
		t.str = arg;
		g_tokens.push_back(t);
		return true;
	}
	// else could be any of them
	if(arg.at(0) == '-')
	{ // command
		t.is_cmd = true;
		if(arg.at(1) == '-')
		{
			// double
			t.is_single = false;
			t.str = arg.substr(2); // the rest is the string
			g_tokens.push_back(t);
			return true;
		}
		else
		{
			// single
			t.is_single = true;
			for(int i = 1; i < arg.size(); i++)
			{
				t.str = arg.at(i);
				g_tokens.push_back(t);
			}
			return true;
		}
	}
	else
	{
		// param
		t.is_cmd = false;
		t.str = arg;
		g_tokens.push_back(t);
		return true;
	}
	// never gets here
	return false;
}

bool cy::argp_begin(int argc, const char **argv)
{
	for(int i = 1; i < argc; i++)
	{
		if(!add_to_token_list(std::string(argv[i])))
			return false;
	}
	if(!process())
	{
		argp_end(); // failed; clear all containers
		return false;
	}
	return true;
}
bool cy::argp_next(cy::argp_arg& next)
{
	if(g_current_arg >= g_processed_args.size())
		return false;
	next = g_processed_args.at(g_current_arg++);
	return true;
}
void cy::argp_end()
{
	g_tokens.clear();
	g_processed_args.clear();
	g_current_arg = 0;
}
