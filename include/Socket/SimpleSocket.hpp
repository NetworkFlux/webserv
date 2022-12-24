#pragma once

#include "../utils.hpp"



/*	This is the Base Socket Class, it handels the creation and deltion
	of not yet connected sockets	*/
class SimpleSocket
{
	// Member Variables
	protected:
		int	_sockFD; // File Descriptor to the Socket

	// Unused Constructors
	private:
		SimpleSocket();
		SimpleSocket(const SimpleSocket& src);

	// Constructors
	public:
		SimpleSocket(int domain, int type, int protocol);
		~SimpleSocket();
		int		get_sock_fd(void);

	// Unused Operator Overload
	private:
		SimpleSocket&	operator=(const SimpleSocket& other);

	// Helper Functions
	protected:
		void	testConnection(int toTest, std::string errorMessage);
};