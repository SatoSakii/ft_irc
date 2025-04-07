/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCUtils.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:06:32 by albernar          #+#    #+#             */
/*   Updated: 2025/04/07 23:15:44 by albernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCUTILS_HPP
# define IRCUTILS_HPP
# include <string>
# include <vector>
# include <sstream>

struct IRCCommand {
	std::string					command;
	std::vector<std::string>	params;
};

class	IRCUtils
{
public:
	static std::vector <std::string>	splitString(const std::string &str, const std::string &delimiter);
	static bool							startsWith(const std::string &str, const std::string &prefix);
	static bool							equalsIgnoreCase(const std::string &str1, const std::string &str2);
};

class IRCResponse
{
public:
	static std::string error(std::string hostname, int code, std::string target, std::string message);
};

IRCCommand	parseIRCCommand(std::string	message);

#endif