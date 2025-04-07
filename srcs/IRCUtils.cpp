/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCUtils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:37:26 by albernar          #+#    #+#             */
/*   Updated: 2025/04/07 23:15:43 by albernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCUtils.hpp"

IRCCommand	parseIRCCommand(std::string	message) {
	IRCCommand			ircCommand;
	std::istringstream	stream(message);
	std::string			word;
	std::string			trailing;
	char				c;

	if (stream >> ircCommand.command) {
		while (stream >> word) {
			if (!word.empty() && word[0] == ':') {
				trailing = word.substr(1);
				while (stream.get(c))
					trailing += c;
				ircCommand.params.push_back(trailing);
				break ;
			}
			ircCommand.params.push_back(word);
		}
	}
	return (ircCommand);
}

std::string	IRCResponse::error(std::string hostname, int code, std::string target, std::string message) {
	std::ostringstream	response;

	response << ":" << hostname << " " << code << " " << target << " :" << message << "\r\n";
	return (response.str());
}

std::vector<std::string>	IRCUtils::splitString(const std::string &str, const std::string &delimiter) {
	std::vector<std::string>	tokens;
	std::string					token;
	size_t						start;
	size_t						end;

	end = 0;
	start = 0;
	while ((end = str.find(delimiter, start)) != std::string::npos) {
		token = str.substr(start, end - start);
		tokens.push_back(token);
		start = end + delimiter.length();
	}
	tokens.push_back(str.substr(start));
	return (tokens);
}

bool	IRCUtils::startsWith(const std::string &str, const std::string &prefix)  {
	return (str.compare(0, prefix.length(), prefix) == 0);
}

bool	IRCUtils::equalsIgnoreCase(const std::string &str1, const std::string &str2) {
	if (str1.length() != str2.length())
		return (false);
	for (size_t i = 0; i < str1.length(); ++i) {
		if (std::tolower(str1[i]) != std::tolower(str2[i]))
			return (false);
	}
	return (true);
}
