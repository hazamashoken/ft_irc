/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abossel <abossel@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 14:31:48 by abossel           #+#    #+#             */
/*   Updated: 2023/03/23 08:56:31 by abossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <string>
# include <map>

class Socket
{
	public:
		Socket(int port, int backlog = 128);
		virtual ~Socket();
		int size();
		int answer();
		void hangup(int fd);
		bool alive(int fd);
		bool writable(int fd);
		bool readable(int fd);
		bool contains(int fd);
		std::string read(int fd);
		bool write(int fd, std::string data);
		bool flush();

	private:
		struct client_s
		{
			std::string send_buffer;
			std::string recv_buffer;
		};
		typedef std::map<int, struct client_s> container_type;

		Socket(const Socket &other); // disable copy constructor
		void init();
		void destroy();
		void error(std::string s);

		int _server;
		int _port;
		int _backlog;
		container_type _clients;
};

#endif
