/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 19:39:01 by albernar          #+#    #+#             */
/*   Updated: 2025/04/08 17:38:56 by albernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

std::string	BotCommands::transformCommand(std::string messageText) {
	std::string		result;
	std::vector<std::string>	_result;

	result = messageText;
	_result = IRCUtils::splitString(result, " ");
	if (_result.size() < 2 || _result[0] != "!transform")
		return (messageText);
	result = _result[1];
	std::reverse(result.begin(), result.end());
	return (result);
}
