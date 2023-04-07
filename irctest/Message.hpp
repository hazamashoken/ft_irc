/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abossel <abossel@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 09:24:28 by abossel           #+#    #+#             */
/*   Updated: 2023/03/28 08:56:41 by abossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <string>

class Message
{
    public:
        bool readMessage(std::string message);
        void printMessage();

    private:
        std::string popFront(std::string message);
        std::string trimFront(std::string message, std::string trim);
        bool readPrefix(std::string message);

        // prefix data
        std::string _servername;
        std::string _nick;
        std::string _user;
        std::string _host;
};

#endif
