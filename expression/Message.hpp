/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abossel <abossel@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 09:24:28 by abossel           #+#    #+#             */
/*   Updated: 2023/04/15 18:43:19 by abossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <string>
# include "Expression.hpp"

# define IRC_INVALID 1
# define IRC_PASS 2
# define IRC_NICK 3
# define IRC_USER 4
# define IRC_OPER 5
# define IRC_MODE 6
# define IRC_SERVICE 7
# define IRC_QUIT 8
# define IRC_SQUIT 9

class Message
{
    public:
        Message();
        virtual ~Message();
        std::string getNickname() const;
        std::string getUser() const;
        std::string getHostname() const;
        std::string getHostaddr() const;
        std::string getHost() const;
        std::string getServername() const;
        std::string getPrefix() const;
        std::string getPassword() const;
        std::string getBitmode() const;
        std::string getUnused() const;
        std::string getRealname() const;
        std::string getMode() const;
        std::string getWildname() const;
        std::string getInfo() const;
        int getMsgType(std::string message);
        std::string getMsgStr() const;

    private:
        void expressionInitHostaddr();
        void expressionInitChannel();
        void expressionInit();
        void messageInit();

        Expression _nicknameExp;
        std::string _nicknameStr;

        Expression _userExp;
        std::string _userStr;

        Expression _hostnameExp;
        std::string _hostnameStr;

        Expression _hostaddrExp;
        std::string _hostaddrStr;

        Expression _hostExp;
        std::string _hostStr;

        Expression _servernameExp;
        std::string _servernameStr;

        Expression _prefixExp;
        std::string _prefixStr;

        Expression _passwordExp;
        std::string _passwordStr;

        Expression _bitmodeExp;
        std::string _bitmodeStr;

        Expression _unusedExp;
        std::string _unusedStr;

        Expression _realnameExp;
        std::string _realnameStr;

        Expression _modeExp;
        std::string _modeStr;

        Expression _wildnameExp;
        std::string _wildnameStr;

        Expression _infoExp;
        std::string _infoStr;

        Expression _channelExp;
        std::string _channelStr;

        std::string _messageStr;

        Expression _passMsgExp;
        Expression _nickMsgExp;
        Expression _userMsgExp;
        Expression _operMsgExp;
        Expression _modeMsgExp;
        Expression _serviceMsgExp;
        Expression _quitMsgExp;
        Expression _squitMsgExp;
};

#endif
