/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCUtils.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:06:32 by albernar          #+#    #+#             */
/*   Updated: 2025/04/04 00:15:58 by albernar         ###   ########.fr       */
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
};

class IRCResponse
{
public:
	static std::string error(std::string hostname, int code, std::string target, std::string message);
};

IRCCommand	parseIRCCommand(std::string	message);

#endif