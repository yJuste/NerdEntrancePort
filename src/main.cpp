/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 18:43:38 by layang            #+#    #+#             */
/*   Updated: 2025/07/24 23:43:24 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "main.h"

std::string getCurrentWorkingDirectory() {
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		return std::string(cwd);
	} else {
		perror("getcwd() error");
		return std::string("");
	}
}

bool fileExists(const std::string &path)
{
	struct stat buffer;
	return(stat(path.c_str(), &buffer) == 0);
}

std::string readFile(const std::string &path)
{
	std::ifstream file(path.c_str());
	if (!file)
		return "";
	std::ostringstream oss;
	std::string line;
	while (std::getline(file, line))
		oss << line << "\n";
	file.close();
	return oss.str();
}

std::string getContentType(const std::string &path)
{
	if (path.length() >= 5 && path.substr(path.length() - 5) == ".html")
		return "text/html";
	if (path.length() >= 4 && path.substr(path.length() - 4) == ".css")
		return "text/css";
	if (path.length() >= 3 && path.substr(path.length() - 3) == ".js")
		return "application/js";
	if (path.length() >= 4 && path.substr(path.length() - 4) == ".jpg")
		return "images/jpeg";
	if (path.length() >= 5 && path.substr(path.length() - 5) == ".jpeg")
		return "images/jpeg";
	if (path.length() >= 4 && path.substr(path.length() - 4) == ".png")
		return "images/png";
	if (path.length() >= 4 && path.substr(path.length() - 4) == ".gif")
		return "images/gif";
	return "text/plain";
}

int	main(void)
{
	int serverFd, newSocket;
	struct sockaddr_in address;
	int opt = 1;
	int addrLen = sizeof(address);
	char buffer[BUFFER_SIZE];

	// Open a socket communication
	serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverFd == 0)
	{
		perror("Socket failed");
		exit(EXIT_FAILURE);
	}

	// Allow your server socket to immediately reuse a port that was previously used after closing (SO_REUSEADDR)
	// , and also allows multiple processes or threads to bind to the same port (SO_REUSEPORT)
	// , improving flexibility and performance.
	if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("Setsocketopt failed");
		exit(EXIT_FAILURE);
	}

	// Set server address
	address.sin_family = AF_INET;  // IPv4
	address.sin_addr.s_addr = INADDR_ANY; // any Network Card
	address.sin_port = htons(PORT);  // change host byte order to big-endian for Network communication

	// Bind socket to address
	if (bind(serverFd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("Bind failed");
		exit(EXIT_FAILURE);		
	}

	// Listen for requests
	if (listen(serverFd, 3) < 0)
	{
		perror("Listen failed");
		exit(EXIT_FAILURE);	
	}
	std::cout << "Web server is running on Port: " << PORT << "..." << std::endl;

	while (true)
	{
		// accept client connections
		newSocket = accept(serverFd, (struct sockaddr *)&address, (socklen_t *)&addrLen);
		if (newSocket < 0)
		{
			perror("Accept failed");
			exit(EXIT_FAILURE);
		}

		// read client requests
		int bytesRead = read(newSocket, buffer, BUFFER_SIZE - 1);
		if (bytesRead < 0) {
			perror("Read failed");
			close(newSocket);
			continue;
		}
		buffer[bytesRead] = '\0';
		std::cout << "Request received: " << std::endl;
		std::cout << buffer << std::endl;
		
		std::istringstream request(buffer);
		std::string method, path, version;
		request >> method >> path >> version;

		// default page: index.html
		std::string root = getCurrentWorkingDirectory();
		std::string filePath;
		if (path == "/")
			filePath = root + "/index.html";
		else
			filePath = root + path;

		// get request
		if (method == "GET")
		{
			if (fileExists(filePath))
			{
				std::string content = readFile(filePath);
				std::string contentType = getContentType(filePath);
				std::ostringstream response;
				response <<  "HTTP/1.1 200 OK\r\n";
				response <<  "Content-Type: " << contentType << "\r\n";
				response <<  "Content-Length: " << content.length() << "\r\n";
				response << "\r\n";
				response << content;
				// send response
				send(newSocket, response.str().c_str(), response.str().length(), 0);
			}
			else
			{
				std::string notFound = "HTTP/1.1 404 Not Found\r\n\r\n404 Not Found";
				send(newSocket, notFound.c_str(), notFound.length(), 0);
			}
		}
		else if (method == "POST")  // post request
		{
			std::string response = "HTTP/1.1 200 OK\r\nContent-Tyoe: text/plain\r\n\r\nPost request";
			send(newSocket, response.c_str(), response.length(), 0);	
		}
		else
		{
			std::string methodNotAllowed = "HTTP/1.1 405 Method Not Allowed\r\n\r\n405 Method Not Allowed";
			send(newSocket, methodNotAllowed.c_str(), methodNotAllowed.length(), 0);	
		}
		close(newSocket);
	}
	close(serverFd);
	return 0;
}
