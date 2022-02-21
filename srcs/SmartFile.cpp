#include "SmartFile.hpp"

SmartFile::SmartFile(void)
{}

SmartFile::SmartFile(std::string const &name, std::string const &mode)
: _name(name), _mode(mode)
{
    _file = fopen(_name.c_str(), _mode.c_str());
    if (!_file)
        throw std::exception();
    else
    {
        fcntl(fileno(_file), F_SETFD, (fcntl(fileno(_file), F_GETFD) | O_NONBLOCK));
        Server::addWatchedFd(fileno(_file));
    }
}

SmartFile::SmartFile(SmartFile const &src)
{
    *this = src;
}

SmartFile::~SmartFile(void)
{
    if (_file)
        fclose(_file);
}

SmartFile &SmartFile::operator=(SmartFile const &rhs)
{
    _file = rhs._file;
    _name = rhs._name;
    _mode = rhs._mode;
    return *this;
}

