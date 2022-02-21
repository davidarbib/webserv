#include "SmartFile.hpp"

SmartFile::SmartFile(void):
{
}

SmartFile(std::string const &name, std::string const &mode)
: _name(name), _mode(mode)
{
 //open
 //change mode non block
 //add server::origin_fds
 //check 
}

SmartFile::SmartFile(SmartFile const &src)
{
}

SmartFile::~SmartFile(void)
{
}

SmartFile	&SmartFile::operator=(SmartFile const &rhs)
{
}

