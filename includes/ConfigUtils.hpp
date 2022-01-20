#ifndef CONFIGUTILS_HPP
# define CONFIGUTILS_HPP

# include <string>
# include <string.h>
# include <vector>
# include <iostream>
# include <fstream>
# include <sstream>

std::string
parse(std::string const &confFile, int pos_start);

std::string
getNextBlock(std::string const &confFile, int pos_start);

std::vector<std::string>
getBlocks(std::string const &confFile, std::string const &block_name);

std::string
read_config(std::string config_path);

#endif
