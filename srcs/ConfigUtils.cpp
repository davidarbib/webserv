# include "ConfigUtils.hpp"

std::string
parse(std::string const &confFile, int pos_start)
{
	int pos_end;

	while (confFile[pos_start] != ' ')
		pos_start++;
	while (confFile[pos_start] == ' ')
	{
		pos_start = confFile.find(' ', pos_start);
		pos_start++;
	}
	pos_end = confFile.find('\n', pos_start);

	return (confFile.substr(pos_start, pos_end - pos_start));
}

std::string
getNextBlock(std::string const &confFile, int pos_start)
{
	int pos_end = pos_start + 1;
	int brackets = 0;
	
	while (confFile[pos_end] && !(confFile[pos_end - 1] == '}' && brackets == 0))
	{
		if (confFile[pos_end] == '{')
			brackets++;
		else if (confFile[pos_end] == '}')
			brackets--;
		pos_end++;
	}
	return (confFile.substr(pos_start, pos_end - pos_start));
}

int
is_block(std::string const &confFile, int pos)
{
	while (confFile[pos] != '\n')
	{
		if (confFile[pos] == '{')
			return (1);
		pos++;
	}
	return (0);
}

std::vector<std::string>
getBlocks(std::string const &confFile, std::string const &block_name)
{
	std::vector<std::string> blocks;

	int pos_start;

	pos_start = confFile.find(block_name);
	while (pos_start != -1)
	{
		if (is_block(confFile, pos_start))
			blocks.push_back(getNextBlock(confFile, pos_start));
		pos_start += 1;
		pos_start = confFile.find(block_name, pos_start);
	}
	return (blocks);
}

std::string
read_config(std::string config_path)
{
	std::stringstream buffer;
	std::ifstream file(config_path.c_str());
	
  	if (file.is_open())
  	{
    	buffer << file.rdbuf();
		file.close();
  	}
	else 
	{
		std::cout << "Unable to open " << config_path << std::endl;
		return ("");
	}
	return (buffer.str());
}
