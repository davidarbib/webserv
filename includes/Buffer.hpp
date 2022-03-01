#ifndef BUFFER_HPP
# define BUFFER_HPP

# include "typedefs.hpp"
# include <string>
# include <iostream>
# include <vector>

class Buffer
{
	private:

		size_t				_idx;
		std::vector<char>	_buffer;

		Buffer(Buffer const &src);
		Buffer &
		operator=(Buffer const &rhs);

	public:

		Buffer(void);

		virtual	~Buffer(void);
		
		void
		setIdx(int);

		void
		incIdx(int);

		int
		getIdx(void) const;

		void
		fillBuffer(char *raw_buffer, int size);

		void
		dumpData(char *dump, int size);

		std::vector<char> &
		getBuffer(void);

		char const *
		getContent(void);

		static bool
		isEndLine(std::string &line, int index);

		void
		clearBuffer(void);
		
		void
		append(std::string const &message);

		void
		append(std::vector<char> const &message);

		void
		eatData(size_t size);

		bool
		isEmpty(void);

		size_t
		size(void);
};
#endif
