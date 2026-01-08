#include "IRC.hpp"


Server::Server(int port, const std::string& password)
	: _port(port),
	  _password(password),
	  _serverName(SERVER_NAME),
	  _serverSocket(-1),
	  _running(false),
	  _cmdHandler(NULL)
{
	   _creationDate = Utils::getCurrentTimestamp();

	   _cmdHandler = new CommandHandler(*this);

	   _pollFds.reserve(MAX_CLIENTS + 1);  // +1 pour le socket serveur
}

// Destructeur
Server::~Server()
{
	   for (std::map<int, Client*>::iterator it = _clients.begin();
	        it != _clients.end(); ++it)
	   {
	       close(it->first);      // Ferme le socket
	       delete it->second;     // Supprime l'objet Client
	   }
	   _clients.clear();

	   for (std::map<std::string, Channel*>::iterator it = _channels.begin();
	        it != _channels.end(); ++it)
	   {
	       delete it->second;
	   }
	   _channels.clear();

	   if (_serverSocket != -1)
	       close(_serverSocket);

	   delete _cmdHandler;
}

/* ========================================================================== */
/*                       INITIALISATION DU SERVEUR                            */
/* ========================================================================== */

/*
** Étapes :
** 1. Créer le socket (socket())
** 2. Configurer les options (setsockopt())
** 3. Mettre en mode non-bloquant (fcntl())
** 4. Lier au port (bind())
** 5. Mettre en écoute (listen())
** 6. Ajouter à poll()
*/
bool Server::init()
{
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket == -1)
	{
	    std::cerr << "Error: socket() failed" << std::endl;
	    return false;
	}

	int opt = 1;
	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
	    std::cerr << "Error: setsockopt() failed" << std::endl;
	    close(_serverSocket);
	    return false;
	}

	if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK) == -1)
	{
	    std::cerr << "Error: fcntl() failed" << std::endl;
	    close(_serverSocket);
	    return false;
	}

	struct sockaddr_in serverAddr;
	std::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;           // IPv4
	serverAddr.sin_addr.s_addr = INADDR_ANY;   // Accepte toutes les interfaces
	serverAddr.sin_port = htons(_port);        // Port en network byte order

	if (bind(_serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
	{
	    std::cerr << "Error: bind() failed - port " << _port << " may be in use" << std::endl;
	    close(_serverSocket);
	    return false;
	}

	if (listen(_serverSocket, MAX_CLIENTS) == -1)
	{
	    std::cerr << "Error: listen() failed" << std::endl;
	    close(_serverSocket);
	    return false;
	}

	addToPoll(_serverSocket);

	return true;
}

/* ========================================================================== */
/*                       BOUCLE PRINCIPALE                                    */
/* ========================================================================== */

// 1. Appeler poll() avec un timeout (ex: 1000ms)
// 2. Parcourir tous les fd surveillés
// 3. Si c'est le socket serveur, accepter nouvelle connexion
// 4. Si c'est un client
// 5. Nettoyer les clients marqués pour déconnexion
void Server::run()
{
	_running = true;
	while (_running)
	{
	    
	    int pollResult = poll(&_pollFds[0], _pollFds.size(), 1000);

	    if (pollResult == -1)
	    {
	        if (errno == EINTR)  // Interrompu par un signal
	            continue;
	        std::cerr << "Error: poll() failed" << std::endl;
	        break;
	    }

	    if (pollResult == 0)
	        continue;

	    for (size_t i = 0; i < _pollFds.size(); ++i)
	    {
	        // Aucun événement sur ce fd
	        if (_pollFds[i].revents == 0)
	            continue;

	        if (_pollFds[i].fd == _serverSocket)
	        {
	            if (_pollFds[i].revents & POLLIN)
	                acceptNewClient();
	        }
	        else
	        {
	            int clientFd = _pollFds[i].fd;

	            // Erreur ou déconnexion
	            if (_pollFds[i].revents & (POLLERR | POLLHUP | POLLNVAL))
	            {
	                disconnectClient(clientFd);
	                continue;
	            }
	            // Données à lire
	            if (_pollFds[i].revents & POLLIN)
	                handleClientData(clientFd);

	            // Prêt à écrire (si des données sont en attente)
	            if (_pollFds[i].revents & POLLOUT)
	                flushClientBuffer(clientFd);
	        }
	    }
	    cleanupDisconnectedClients();
	}
}

void Server::stop()
{
    _running = false;
}

