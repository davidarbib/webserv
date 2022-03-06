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
    else if (mode == "r")
        _file = open(_name.c_str(), O_RDONLY | O_NONBLOCK);
    else if (mode == "w")
        _file = open(_name.c_str(), O_WRONLY | O_NONBLOCK | O_CREAT, 0644);
    if (_file < 1)
        throw std::exception();
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
SmartFile::gets(char *buf, int size) const
{
	struct timeval				tv;
	tv.tv_sec = DELAY;
	tv.tv_usec = 0;
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(_file, &fds);
	if (select(_file + 1, &fds, 0, 0, &tv) == 1)
		return read(_file, buf, size);
	else
		return 0;
}

int
SmartFile::puts(const char *buf, int size)
{
	struct timeval				tv;
	tv.tv_sec = DELAY;
	tv.tv_usec = 0;
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(_file, &fds);
	if (select(_file + 1, 0, &fds, 0, &tv) == 1)
		return write(_file, buf, size);
	else
		return 0;
}
