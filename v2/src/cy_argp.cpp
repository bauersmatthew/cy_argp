/*
 * File: cy_argp.cpp
 * Edited: 04 Dec 2015
 * Author: Matthew Bauer
 */

#include "cy_argp.h"
#include <stdexcept.h>

// settings
bool g_allow_lone_params = true;

// command registry
std::map<uint32_t, cy::argp_arg_extra_info> g_cmd_registry;

// tokenized arg list
std::vector<cy::argp_arg> g_arg_list;

// misc functions
cy::argp_arg_extra_info make_info_struct(const std::string& cmd, cy::argp_cmd_type cmd_type, cy::argp_param_type param_type)
{
	cy::argp_arg_extra_info ret;
	ret.cmd = cmd;
	ret.cmd_type = cmd_type;
	ret.param_type = param_type;
	return ret;
}
cy::argp_arg make_arg_struct(int32_t uid, cy::argp_arg_extra_info info) // note:  must set .param.* seperately
{
	cy::argp_arg ret;
	ret.uid = uid;
	ret.info = info;
	return ret;
}

// settings functions
void cy::argp_ss_allow_lone_params(bool s)
{
	g_allow_lone_params = s;
}

// registry handling functions
bool uid_is_reserved(int32_t uid)
{
	switch(uid)
	{
		case cy::ARGP_UID_NO_CMD:
			return true;
	}
	return false;
}
void cy::argp_register(const std::string& cmd, int32_t uid, argp_cmd_type cmd_type, argp_param_type param_type)
{
	// check arguments
	if(cmd.empty())
		throw std::runtime_error("no command given");
	if(cmd_type == ARGP_CT_SINGLE && cmd.size() != 1)
		throw std::runtime_error("command of type 'single' cannot contain multiple characters");

	// check for uid uniqueness
	if(g_cmd_registry.find(uid) != g_cmd_registry::end)
		throw std::runtime_error("given uid not unique");
	if(uid_is_reserved(uid))
		throw std::runtime_error("given uid is reserved!");
	
	// check for command uniqueness
	for(std::map<int32_t, cy::argp_arg_extra_info>::iterator it = g_cmd_registry.begin(); it != g_cmd_registry.end(); it++)
	{
		if(cmd == it->second.cmd && cmd_type == it->second.cmd_type)
			throw std::runtime_error("equivalent command already registered");
	}

	// construct the info struct
	cy::argp_arg_extra_info aaei = make_info_struct(cmd, cmd_type, param_type);

	// put in registry
	g_cmd_registry.emplace(uid, aaei);
}
void cy::argp_clear_registry()
{
	g_cmd_registry.clear();
}

// arg processing functions
bool cmd_is_registered(const std::string& cmd, cy::argp_cmd_type cmd_type)
{
	for(std::map<int32_t, argp_arg_extra_info>::iterator it = g_cmd_registry.begin(); it != g_cmd_registry.end(); i++)
	{
		if(cmd == it->second.cmd && cmd_type == it->second.cmd_type)
			return true; // found
	}
	return false; // not found
}
bool conver
bool tokenize_and_add_argv_arg(const std::string& arg)
{
	// there are a couple "illegals"
	if(arg == "--" || arg == "-")
		return false;
	
	cy::argp_arg tok_arg;
	
	// take care of lone params
	if(arg.empty() || arg.at(0) != '-')
	{
		if(g_allow_lone_params)
		{
			// make + add
			tok_arg = make_arg_struct(
				uid,
				make_info_struct("", cy::ARGP_CT_NONE, cy::ARGP_PT_STRING));
			tok_arg.param.vstr = "";
			g_arg_list.push_back(tok_arg);
			return true;
		}
		else
			return false;
	}
	
	// process commands
	if(arg.at(1) != '-') // arg.size() >= 2 guaranteed
	{ // single
		for(int i = 1; i < arg.size(); i++)
		{
			// check
			
			
			// make + add each option in string
			
		}
	}
}
bool cy::argp_begin(int argc, char **argv)
{
	
}
