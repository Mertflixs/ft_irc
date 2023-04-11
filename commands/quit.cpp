#include "Command.hpp"


string merge(vector<string>& words) {
	int i;
	std::string result;

	i = 0;
    for (std::vector<std::string>::const_iterator it = words.begin(); it != words.end(); ++it) {
        if (it != words.end() && i > 0) {
            result += words[i];
			if (it != words.end() - 1)
				result += " ";
        }
		i++;
    }
    return result;
}

void Command::quit(int soc, Server &serv, vector<string> &words) {
    Client *cli;
    vector<string> usrChnls;
    vector<Channel*> allChannels;
    string cmd;
    vector<string>::iterator x;
    Server::poll_iterator it;
    Server::channel_iterator ct;
    Server::client_iterator cit;
    Channel::chnlUsersit tic;

    cmd = "Disconnected";
    cli = (serv.getClients())[soc];
    if (cli != NULL) {
        usrChnls = cli->getChannels();
        allChannels = serv.getChannel();
        for(it = serv.getPoll().begin(); it != serv.getPoll().end(); it++) {
            if (it->fd == soc)
                break;
        }
        if (it != serv.getPoll().end()) {
            if (words.size() > 1)
                cmd = merge(words);
            if (usrChnls.size() > 0) {
                for(ct = allChannels.begin(); ct != allChannels.end(); ct++) {
                    for (x = cli->channels.begin(); x != cli->channels.end(); x++) {
                        if ((*x) == (*ct)->channelName) {
                            serv.notifyAll((*ct), *cli, cmd);
                            for(tic = (*ct)->users.begin(); tic != (*ct)->users.end(); tic++) {
                                if (cli->nickName == (*tic).first->nickName) {
                                    (*ct)->users.erase(tic);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        close(cli->soc_fd);
    }
}