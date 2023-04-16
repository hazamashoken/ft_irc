/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <earth78203@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 09:24:28 by abossel           #+#    #+#             */
/*   Updated: 2023/04/16 00:03:31 by tliangso         ###   ########.fr       */
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
# define IRC_JOIN 10
# define IRC_PART 11
# define IRC_TOPIC 12
# define IRC_NAMES 13
# define IRC_LIST 14
# define IRC_INVITE 15
# define IRC_KICK 16
# define IRC_VERSION 17
# define IRC_STATS 18
# define IRC_LINKS 19
# define IRC_TIME 20
# define IRC_CONNECT 21
# define IRC_TRACE 22
# define IRC_ADMIN 23
# define IRC_INFO 24
# define IRC_PRIVMSG 25
# define IRC_NOTICE 26
# define IRC_WHO 27
# define IRC_WHOIS 28
# define IRC_WHOWAS 29
# define IRC_KILL 30
# define IRC_PING 31
# define IRC_PONG 3

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
		// todo: add all the other messages
		Expression _joinMsgExp;
		Expression _partMsgExp;
		Expression _topicMsgExp;
		Expression _namesMsgExp;
		Expression _listMsgExp;
		Expression _inviteMsgExp;
		Expression _kickMsgExp;
		Expression _versionMsgExp;
		Expression _statsMsgExp;
		Expression _linksMsgExp;
		Expression _timeMsgExp;
		Expression _connectMsgExp;
		Expression _traceMsgExp;
		Expression _adminMsgExp;
		Expression _infoMsgExp;
		Expression _privmsgMsgExp;
		Expression _noticeMsgExp;
		Expression _whoMsgExp;
		Expression _whoisMsgExp;
		Expression _whowasMsgExp;
		Expression _killMsgExp;
		Expression _pingMsgExp;
		Expression _pongMsgExp;
		
};

#endif
