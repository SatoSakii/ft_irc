/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:20:33 by albernar          #+#    #+#             */
/*   Updated: 2025/04/04 19:21:10 by albernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include <map>

# include "Client.hpp"

class Channel
{
private:
	std::string				name;
	std::map<int, Client*>	clients;
	std::vector<Client *>	invitedClients;
	std::vector<Client *>	clientsOperator;
	int						maxClients; // +l <limit> | -l
	bool					inviteOnly; // +i | -i
	bool					topicSet; // +t | -t {true = only operators can set topic | false = everyone can set topic}
	std::string				password; // +k <password> | -k = ""
	std::string				topic; // TOPIC <#channel> [<topic>] | topic = ""
public:
	Channel(std::string name);

	void		addClient(Client *client);
	void		addInvitedClient(Client *client);
	void		removeInvitation(Client *client);
	void		removeClient(Client *client);
	void		removeOperator(Client *client);
	bool		isClientInChannel(Client *client) const;
	bool		isInviteOnly(void) const;
	bool		isInvited(Client *client) const;
	bool		isPasswordProtected(void) const;
	bool		isFull(void) const;
	bool		isOperator(Client *client) const;
	void		setClientOperator(Client *client);
	void		setTopic(std::string topic);
	std::string	getTopic(void) const;
	void		setTopicSet(bool topicSet);
	bool		getTopicSet(void) const;
	std::string	getPassword(void) const;
	std::string getUserList(void) const;
	std::map<int, Client *> getClients(void) const;
	std::vector<Client *> getInvitedClients(void) const;

	

	void	broadcastMessage(Client *client, std::string message) const;
};

#endif