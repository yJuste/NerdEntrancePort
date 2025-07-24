// ************************************************************************** //
//                                                                            //
//                main.h                                                      //
//                Created on  : xxx Jul xx xx:xx:xx 2025                      //
//                Last update : xxx Jul xx xx:xx:xx 2025                      //
//                Made by     :                                               //
//                                                                            //
// ************************************************************************** //

#ifndef MAIN_H
# define MAIN_H

// Standard Libraries

# include <iostream>
# include <string>
# include <cstdlib>
# include <sstream>  //std::istringstream, std::ostringstream
# include <fstream>  //std::ifstream, std::ofstream
# include <netinet/in.h> //Internet protocol family, such as IPv4, TCP, and UDP on Linux/Unix systems
# include <unistd.h>
# include <limits.h>
# include <sys/stat.h> //stat(), fstat(), and mkdir(), file attributes: permissions, file types, size, timestamps...


// ************************************************************************** //
//                                  Dependences                               //
// ************************************************************************** //

// Port number
# define PORT 8081

# define BUFFER_SIZE 4096

#endif
