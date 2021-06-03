bool	is_end_line(char *line, int index)
{
	if (line[index] == '\r')
	{
		if (line[index + 1] == '\n')
			return true;
	}
	return false;
}

bool is_end_section(char *line, int index)
{
	if (is_end_line(line, index))
	{
		if (is_end_line(line, index + 2))
			return true;
	}
	return false;
}

// int		get_start_line(char *raw_request, Request &request)
// {
	

// 	return i;
// }