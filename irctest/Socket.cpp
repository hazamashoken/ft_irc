/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abossel <abossel@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 14:45:20 by abossel           #+#    #+#             */
/*   Updated: 2023/03/23 09:20:05 by abossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include <stdexcept>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#define RECV_SIZE 1024
#define SEND_SIZE 1024

Socket::Socket(int port, int backlog)
{
	if (port < 0 || port > 65535)
		throw (std::out_of_range("port number out of range"));
	if (backlog < 0)
		throw (std::out_of_range("backlog number out of range"));
	_server = -1;
	_port = port;
	_backlog = backlog;
	init();
}

Socket::~Socket()
{
	destroy();
}

/*
 * create and bind socket
 */
void Socket::init()
{
	struct sockaddr_in6 addr;
	int on = 1;
	int ret;

	// open the IPv4 and IPv6 socket
	_server = socket(AF_INET6, SOCK_STREAM, 0);
	if (_server == -1)
		error("init(): socket() failed");
	// make socket reusable
	// this prevents waiting for TTL to expire when restarting server
	// FIX: maybe remove in final version as it can make TCP less reliable
	ret = setsockopt(_server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if (ret == -1)
		error("init(): setsockopt() failed");
	// make socket non-blocking
	ret = fcntl(_server, F_SETFL, O_NONBLOCK);
	if (ret == -1)
		error("init(): fcntl() failed");
	// bind the socket
	memset(&addr, 0, sizeof(addr));
	addr.sin6_family = AF_INET6;
	addr.sin6_addr = in6addr_any;
	addr.sin6_port = htons(_port);
	ret = bind(_server, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
	if (ret == -1)
		error("init(): bind() failed");
	ret = listen(_server, _backlog);
	if (ret == -1)
		error("init(): listen() failed");
}

/*
 * close all client connections and close socket
 */
void Socket::destroy()
{
	container_type::iterator it;

	for (it = _clients.begin(); it != _clients.end(); it++)
		close(it->first);
	_clients.clear();
	if (_server != -1)
		close(_server);
	_server = -1;
}

/*
 * check if fd is a client
 * returns true if found or false otherwise
 */
bool Socket::contains(int fd)
{
	return (_clients.count(fd) != 0);
}

/*
 * clean up and throw a runtime error
 */
void Socket::error(std::string s)
{
	destroy();
	perror(NULL);
	throw (std::runtime_error(s));
}

/*
 * returns the number of clients
 */
int Socket::size()
{
	return (_clients.size());
}

/*
 * accept a waiting client connection request
 * returns a file descriptor on success or 0 on failure
 */
int Socket::answer()
{
	struct client_s client;
	struct pollfd pfd;
	int num;
	int fd;

	// poll will get the number of waiting connections
	pfd.fd = _server;
	pfd.events = POLLIN;
	num = poll(&pfd, 1, 0);
	if (num == -1)
		error("answer(): poll() failed");
	if (num == 0)
		return (-1);
	fd = accept(_server, NULL, NULL);
	if (fd == -1)
		error("answer(): accept() failed");
	_clients.insert(std::make_pair(fd, client));
	return (fd);
}

/*
 * close a client connection
 */
void Socket::hangup(int fd)
{
	container_type::iterator it;

	it = _clients.find(fd);
	if (it == _clients.end())
		return ;
	close(fd);
	_clients.erase(it);
}

/*
 * check a client if it is still alive
 * check for client hangup, client error or closed fd
 * returns true if active or false otherwise
 */
bool Socket::alive(int fd)
{
	struct pollfd pfd;
	int num;

	if (!contains(fd))
		return (false);
	pfd.events = POLLHUP | POLLERR | POLLNVAL;
	pfd.fd = fd;
	num = poll(&pfd, 1, 0);
	if (num == -1)
		error("alive(): poll() failed");
	if (num != 0)
		return (false);
	return (true);
}

/*
 * check if a client is ready for writing
 * returns true if ready or false other wise
 */
bool Socket::writable(int fd)
{
	struct pollfd pfd;
	int num;

	if (!contains(fd))
		return (false);
	memset(&pfd, 0, sizeof(pfd));
	pfd.events = POLLOUT;
	pfd.fd = fd;
	num = poll(&pfd, 1, 0);
	if (num == -1)
		error("ready(): poll() failed");
	if (num != 0)
		return (true);
	return (false);
}

/*
 * check if a client has data to read
 * returns true if client has data or false other wise
 */
bool Socket::readable(int fd)
{
	container_type::iterator it;
	struct pollfd pfd;
	int num;

	it = _clients.find(fd);
	if (it == _clients.end())
		return (false);

	// if the string buffer still has data return true
	if (it->second.recv_buffer.size() != 0)
		return (true);

	memset(&pfd, 0, sizeof(pfd));
	pfd.events = POLLIN;
	pfd.fd = fd;
	num = poll(&pfd, 1, 0);
	if (num == -1)
		error("check(): poll() failed");
	if (num != 0)
		return (true);
	return (false);
}

/*
 * read until new line or end of data
 * returns a string with the read data
 * returns "" if no data to read or error
 * the string may contain '\0' chracters
 */
std::string Socket::read(int fd)
{
	container_type::iterator it;
	std::string str;
	size_t position;
	char array[RECV_SIZE];
	ssize_t size;

	it = _clients.find(fd);
	if (it == _clients.end())
		return ("");

	size = 1;
	while (size > 0)
	{
		// if the string buffer has new line return the line
		// the remainder stays in the buffer
		position = it->second.recv_buffer.find('\n');
		if (position != std::string::npos)
		{
			// append the line to the return string
			str.append(it->second.recv_buffer, 0, position + 1);
			// remove the line from the receive buffer
			it->second.recv_buffer = it->second.recv_buffer.substr(position + 1);
			break;
		}
		// no new line so append receive buffer to return string
		str.append(it->second.recv_buffer);
		it->second.recv_buffer.clear();
		// try to receive more data
		// must poll before every recv
		// receive buffer should be clear before this
		size = 0;
		if (readable(fd))
			size = recv(fd, array, RECV_SIZE, 0);
		// append received data to receive buffer
		if (size > 0)
			it->second.recv_buffer.append(array, size);
	}
	return (str);
}

/*
 * write until end of data or client refuses
 * returns true if all data sent
 * remaining data is stored in send buffer
 * the data string may contain '\0' chracters
 */
bool Socket::write(int fd, std::string data)
{
	container_type::iterator it;
	std::string str;
	char array[SEND_SIZE];
	size_t array_size;
	ssize_t size;

	it = _clients.find(fd);
	if (it == _clients.end())
		return (false);

	// append data to current send buffer
	it->second.send_buffer.append(data);
	size = 1;
	// loop while previous send successful and send buffer has data
	while (size > 0 && it->second.send_buffer.size() > 0)
	{
		array_size = it->second.send_buffer.size();
		if (array_size > SEND_SIZE)
			array_size = SEND_SIZE;
		// copy from send buffer to char array
		// use memcpy instead of strncpy because data string may contain \0
		memcpy(array, it->second.send_buffer.c_str(), array_size);
		// try to send the data
		// must poll before every send
		size = 0;
		if (writable(fd))
			size = send(fd, array, array_size, 0);
		if (size <= 0)
			break;
		// send successful so remove data from send buffer
		it->second.send_buffer = it->second.send_buffer.substr(size);
	}
	// if send buffer is empty return true
	if (it->second.send_buffer.size() == 0)
		return (true);
	return (false);
}

/*
 * attempt to flush all send buffers
 * calls write(fd, "") on each client to try sending again
 * returns true if all flushed or false otherwise
 */
bool Socket::flush()
{
	container_type::iterator it;
	bool result;

	result = true;
	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->second.send_buffer.size() != 0)
			if (write(it->first, "") == false)
				result = false;
	}
	return (result);
}
