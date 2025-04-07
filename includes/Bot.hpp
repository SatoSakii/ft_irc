/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 19:39:17 by albernar          #+#    #+#             */
/*   Updated: 2025/04/07 21:11:01 by albernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP
# include "CommandHandler.hpp"
# include "Server.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "IRCUtils.hpp"
# include "CommandHandler.hpp"

class BotCommands
{
public:
	static std::string	transformCommand(std::string messageText);
};

#endif