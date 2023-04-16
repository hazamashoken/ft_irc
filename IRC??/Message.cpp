/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <earth78203@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 09:33:08 by abossel           #+#    #+#             */
/*   Updated: 2023/04/16 00:06:53 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include <iostream>

#define IRC_SPECIAL std::string("-[]\\`^{}")
#define IRC_NONUSER std::string("\0\r\n @", 5)
#define IRC_NONPASS std::string("\0\r\n \t", 5)
#define IRC_NONPARAM std::string("\0\r\n :", 5)
#define IRC_NONREAL std::string("\0\r\n:", 4)
#define IRC_NONCHANNEL std::string("\0\a\r\n ,:")

Message::Message()
{
    expressionInit();
    messageInit();
}

Message::~Message()
{
}

void Message::expressionInitHostaddr()
{
    Expression ip4byte;
    ip4byte.exp(Expression().any("2").any("5").any("012345")).jmp();
    ip4byte.exp(Expression().any("2").any("01234").digit()).jmp();
    ip4byte.exp(Expression().any("01").digit(2, 2)).jmp();
    ip4byte.digit(1, 2);

    Expression ip4addr;
    ip4addr.exp(ip4byte).any(".").exp(ip4byte).any(".").exp(ip4byte).any(".").exp(ip4byte);

    Expression ip6pre;
    ip6pre.xdigit(1, 4).any(":");
    Expression ip6suf;
    ip6suf.any(":").xdigit(1, 4);

    Expression ip6pre0;
    ip6pre0.any("0", 1, 4).any(":");
    Expression ip6suf0;
    ip6suf0.any(":").any("0", 1, 4);
    Expression ip6ffff;
    ip6ffff.any(":").any("fF", 4, 4).any(":");

    // IPv6 addresses
    Expression ip6addr;

    // IPv4 mapped addresses 0:0:0:0:0:ffff:a.b.c.d
    ip6addr.exp(Expression().exp(ip6pre0, 4, 4).any("0", 1, 4).exp(ip6ffff).exp(ip4addr)).jmp();
    ip6addr.exp(Expression().exp(ip6pre0, 1, 4).exp(ip6ffff).exp(ip4addr)).jmp();
    ip6addr.exp(Expression().any(":").exp(ip6suf0, 0, 4).exp(ip6ffff).exp(ip4addr)).jmp();

    // IPv4 translated addresses 0:0:0:0:ffff:0:a.b.c.d
    ip6addr.exp(Expression().exp(ip6pre0, 3, 3).any("0", 1, 4).exp(ip6ffff).exp(ip6pre0).exp(ip4addr)).jmp();
    ip6addr.exp(Expression().exp(ip6pre0, 1, 3).exp(ip6ffff).exp(ip6pre0).exp(ip4addr)).jmp();
    ip6addr.exp(Expression().any(":").exp(ip6suf0, 0, 3).exp(ip6ffff).exp(ip6pre0).exp(ip4addr)).jmp();

    // IPv4 standard addresses 0:0:0:0:0:0:0:0
    ip6addr.exp(Expression().exp(ip6pre, 7, 7).xdigit(1, 4)).jmp();
    ip6addr.exp(Expression().exp(ip6pre, 6, 6).exp(ip6suf, 1, 1)).jmp();
    ip6addr.exp(Expression().exp(ip6pre, 5, 5).exp(ip6suf, 1, 2)).jmp();
    ip6addr.exp(Expression().exp(ip6pre, 4, 4).exp(ip6suf, 1, 3)).jmp();
    ip6addr.exp(Expression().exp(ip6pre, 3, 3).exp(ip6suf, 1, 4)).jmp();
    ip6addr.exp(Expression().exp(ip6pre, 2, 2).exp(ip6suf, 1, 5)).jmp();
    ip6addr.exp(Expression().exp(ip6pre, 1, 1).exp(ip6suf, 1, 6)).jmp();
    ip6addr.exp(Expression().any(":").exp(ip6suf, 1, 7)).jmp();
    ip6addr.exp(Expression().exp(ip6pre, 1, 7).any(":")).jmp();

    // special case ::
    ip6addr.all("::");

    _hostaddrExp = Expression(_hostaddrStr);
    _hostaddrExp.exp(ip4addr).jmp().exp(ip6addr);
}

void Message::expressionInitChannel()
{
    Expression channelID;
    channelID.all("!").upper(5, 5).add("0123456789");

    Expression channel;
    channel.exp(channelID).jmp();
    channel.any("#+&").con();
    channel.inv(IRC_NONCHANNEL, 1);
    channel.exp(Expression().any(":").inv(IRC_NONCHANNEL, 1), 0, 1);
}

void Message::expressionInit()
{
    // nickname expression
    _nicknameExp = Expression(_nicknameStr);
    _nicknameExp.alpha().add(IRC_SPECIAL).exp(Expression().alnum().add(IRC_SPECIAL).add("-"), 0, 8);

    // username expression
    _userExp = Expression(_userStr);
    _userExp.inv(IRC_NONUSER, 1);

    // hostname expression
	Expression shortnameExp;
	shortnameExp.alnum(1).exp(Expression().any("-", 0).alnum(1), 0);
    _hostnameExp = Expression(_hostnameStr);
	_hostnameExp.exp(shortnameExp).exp(Expression().any(".").exp(shortnameExp, 1), 0);

    // hostaddr expression
    expressionInitHostaddr();

    // host expression
    _hostExp = Expression(_hostStr);
    _hostExp.exp(_hostnameExp).jmpe().exp(_hostaddrExp);

    // servername expression
    _servernameExp = Expression(_hostnameExp, _servernameStr);

    // prefix expression
    _prefixExp = Expression(_prefixStr);
    _prefixExp.exp(Expression().exp(_nicknameExp).any("!").exp(_userExp).any("@").exp(_hostExp)).jmp();
	_prefixExp.exp(Expression().exp(_nicknameExp).any("@").exp(_hostExp)).jmp();
	_prefixExp.exp(_nicknameExp).jmpe();
    _prefixExp.exp(_servernameExp);

    // password expression
    _passwordExp = Expression(_passwordStr);
    _passwordExp.inv(IRC_NONPASS, 5);

    // bitmode expression
    _bitmodeExp = Expression(_bitmodeStr);
    _bitmodeExp.digit();

    // unused expression
    _unusedExp = Expression(_unusedStr);
    _unusedExp.inv(IRC_NONPARAM, 1);

    // realname expression
    _realnameExp = Expression(_realnameStr);
    _realnameExp.inv(IRC_NONREAL, 1);

    // mode expression
    _modeExp = Expression(_modeStr);
    _modeExp.any("+-").any("iwoOrs");

    // wildname expression
    _wildnameExp = Expression(_wildnameStr);
    _wildnameExp.alnum(1).add(".*?\\");

    // info expression
    _infoExp = Expression(_infoStr);
    _infoExp.inv(IRC_NONREAL, 1);
}

void Message::messageInit()
{
    // PASS message
    _passMsgExp = Expression(_messageStr);
    _passMsgExp.all("PASS ").exp(_passwordExp).all("\r\n");

    // NICK message
    _nickMsgExp = Expression(_messageStr);
    _nickMsgExp.all("NICK ").exp(_nicknameExp).all("\r\n");

    // USER message
    _userMsgExp = Expression(_messageStr);
    _userMsgExp.all("USER ").exp(_userExp);
    _userMsgExp.all(" ").exp(_bitmodeExp);
    _userMsgExp.all(" ").exp(_unusedExp);
    _userMsgExp.all(" :").exp(_realnameExp).all("\r\n");

    // OPER message
    _operMsgExp = Expression(_messageStr);
    _operMsgExp.all("OPER ").exp(_userExp);
    _operMsgExp.all(" ").exp(_passwordExp).all("\r\n");

    // MODE message
    _modeMsgExp = Expression(_messageStr);
    _modeMsgExp.all("MODE ").exp(_modeExp).all("\r\n");

    // SERVICE message
    _serviceMsgExp = Expression(_messageStr);
    _serviceMsgExp.all("SERVICE ").exp(_nicknameExp);
    _serviceMsgExp.all(" ").exp(_unusedExp);
    _serviceMsgExp.all(" ").exp(_wildnameExp);
    _serviceMsgExp.all(" ").exp(_unusedExp);
    _serviceMsgExp.all(" ").exp(_unusedExp);
    _serviceMsgExp.all(" :").exp(_infoExp).all("\r\n");

    // QUIT message
    _quitMsgExp = Expression(_messageStr);
    _quitMsgExp.all("QUIT :").exp(_infoExp).all("\r\n");

    // SQUIT message
    _squitMsgExp = Expression(_messageStr);
    _squitMsgExp.all("SQUIT ").exp(_servernameExp);
    _squitMsgExp.all(" :").exp(_infoExp).all("\r\n");

	// JOIN message
	_joinMsgExp = Expression(_messageStr);
	_joinMsgExp.all("JOIN ").exp(_channelExp).all("\r\n");

	// PART message
	_partMsgExp = Expression(_messageStr);
	_partMsgExp.all("PART ").exp(_channelExp);
	_partMsgExp.all(" :").exp(_infoExp).all("\r\n");

	// TOPIC message
	_topicMsgExp = Expression(_messageStr);
	_topicMsgExp.all("TOPIC ").exp(_channelExp);
	_topicMsgExp.all(" :").exp(_infoExp).all("\r\n");

	// NAMES message
	_namesMsgExp = Expression(_messageStr);
	_namesMsgExp.all("NAMES ").exp(_channelExp).all("\r\n");

	// LIST message
	_listMsgExp = Expression(_messageStr);
	_listMsgExp.all("LIST ").exp(_channelExp).all("\r\n");

	// INVITE message
	_inviteMsgExp = Expression(_messageStr);
	_inviteMsgExp.all("INVITE ").exp(_nicknameExp);
	_inviteMsgExp.all(" ").exp(_channelExp).all("\r\n");

	// KICK message
	_kickMsgExp = Expression(_messageStr);
	_kickMsgExp.all("KICK ").exp(_channelExp);
	_kickMsgExp.all(" ").exp(_nicknameExp);
	_kickMsgExp.all(" :").exp(_infoExp).all("\r\n");

	// PRIVMSG message
	_privmsgMsgExp = Expression(_messageStr);
	_privmsgMsgExp.all("PRIVMSG ").exp(_prefixExp);
	_privmsgMsgExp.all(" :").exp(_infoExp).all("\r\n");

	// NOTICE message
	_noticeMsgExp = Expression(_messageStr);
	_noticeMsgExp.all("NOTICE ").exp(_prefixExp);
	_noticeMsgExp.all(" :").exp(_infoExp).all("\r\n");

	// PING message
	_pingMsgExp = Expression(_messageStr);
	_pingMsgExp.all("PING ").exp(_servernameExp).all("\r\n");

	// PONG message
	_pongMsgExp = Expression(_messageStr);
	_pongMsgExp.all("PONG ").exp(_servernameExp).all("\r\n");

}

std::string Message::getNickname() const
{
    return (_nicknameStr);
}

std::string Message::getUser() const
{
    return (_userStr);
}

std::string Message::getHostname() const
{
    return (_hostaddrStr);
}

std::string Message::getHostaddr() const
{
    return (_hostaddrStr);
}

std::string Message::getHost() const
{
    return (_hostStr);
}

std::string Message::getServername() const
{
    return (_servernameStr);
}

std::string Message::getPrefix() const
{
    return (_prefixStr);
}

std::string Message::getPassword() const
{
    return(_passwordStr);
}

std::string Message::getBitmode() const
{
    return(_bitmodeStr);
}

std::string Message::getUnused() const
{
    return(_unusedStr);
}

std::string Message::getRealname() const
{
    return(_realnameStr);
}

std::string Message::getMode() const
{
    return(_modeStr);
}

std::string Message::getWildname() const
{
    return(_wildnameStr);
}

std::string Message::getInfo() const
{
    return(_infoStr);
}

int Message::getMsgType(std::string message)
{
    if (_passMsgExp.match(message))
        return (IRC_PASS);
    if (_nickMsgExp.match(message))
        return (IRC_NICK);
    if (_userMsgExp.match(message))
        return (IRC_USER);
    if (_operMsgExp.match(message))
        return (IRC_OPER);
    if (_modeMsgExp.match(message))
        return (IRC_MODE);
    if (_serviceMsgExp.match(message))
        return (IRC_SERVICE);
    if (_quitMsgExp.match(message))
        return (IRC_QUIT);
    if (_squitMsgExp.match(message))
        return (IRC_SQUIT);
    return (IRC_INVALID);
}

std::string Message::getMsgStr() const
{
    return (_messageStr);
}
