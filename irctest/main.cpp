/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abossel <abossel@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 19:46:59 by abossel           #+#    #+#             */
/*   Updated: 2023/03/23 09:17:42 by abossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include <iostream>
#include <string>
#include <unistd.h>

int main(int argc, char **argv)
{
	std::string str;
	Socket *socket;
	int port;

	if (argc != 2)
	{
		std::cerr << "usage" << std::endl;
		std::cerr << "irctest <port>" << std::endl;
		std::cerr << std::endl;
		std::cerr << "test with" << std::endl;
		std::cerr << "nc 127.0.0.1 <port>" << std::endl;
		return (0);
	}
	port = std::stoi(argv[1]);
	socket = new Socket(port);
	int client;
	client = socket->answer();
	while (client == -1)
	{
		std::cout << "waiting" << std::endl;
		sleep(1);
		client = socket->answer();
	}
	std::cout << "connected" << std::endl;
	while (socket->alive(client))
	{
		if (socket->readable(client))
		{
			str = socket->read(client);
			std::cout << "read: " << str.size() << " bytes " << str;
			socket->write(client, str);
			std::cout << "write: " << str.size() << " bytes" << std::endl;
		}
		sleep(1);
	}
	std::cout << "terminated" << std::endl;
	delete socket;
	return (0);
}
