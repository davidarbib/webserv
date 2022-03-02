#include "SmartFile.hpp"

SmartFile::SmartFile(void)
{}

SmartFile::SmartFile(std::string const &name, std::string const &mode)
: _name(name), _mode(mode)
{
    if (mode == "t")
    {
        bzero(_tmpname, sizeof(_tmpname));
        strncpy(_tmpname, _name.c_str(), name.size());
        strncpy(_tmpname + name.size(), TMP_SUFFIX, TMP_SUFFIX_LEN);
        _file = mkstemp(_tmpname);
        fcntl(_file, F_SETFD, (fcntl(_file, F_GETFD) | O_NONBLOCK));
    }
    else
        _file = open(_name.c_str(), O_RDWR | O_NONBLOCK);
    if (_file < 1)
        throw std::exception();
    else
    {
        Server::addWatchedFd(_file);
    }
}

SmartFile::SmartFile(SmartFile const &src)
{
    *this = src;
}

SmartFile::~SmartFile(void)
{
    if (_mode == "t")
        unlink(_tmpname);
    else
        close(_file);
}

SmartFile &SmartFile::operator=(SmartFile const &rhs)
{
    _file = rhs._file;
    _name = rhs._name;
    _mode = rhs._mode;
    return *this;
}

int
SmartFile::gets(char *buf, int size)
{
	if (Server::isThereSomethingToRead(_file))
		return read(_file, buf, size);
	else
		return 0;
}

int
SmartFile::puts(const char *buf, int size)
{
	if (Server::isPossibleToWrite(_file))
		return write(_file, buf, size);
	else
		return 0;
}
