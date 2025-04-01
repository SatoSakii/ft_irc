/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCUtils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:37:26 by albernar          #+#    #+#             */
/*   Updated: 2025/04/01 17:55:08 by albernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCUtils.hpp"

IRCCommand	parseIRCCommand(std::string	message) {
	IRCCommand			ircCommand;
	std::istringstream	stream(message);
	std::string			word;
	std::string			trailing;

	if (stream >> ircCommand.command) {
		while (stream >> word) {
			if (!word.empty() && word[0] == ':') {
				trailing = word.substr(1);
				while (stream >> word)
					trailing += " " + word;
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
