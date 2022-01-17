#ifndef BUFFER_HPP
# define BUFFER_HPP

# include "typedefs.hpp"
# include <string>
# include <iostream>

class Buffer
{
	private:

		int						_idx;
		std::string				_in_buffer;

		Buffer(Buffer const &src);

	public:

		Buffer(void);

		virtual	~Buffer(void);
		
		Buffer &
		operator=(Buffer const &rhs);

		void
		setIdx(int);

		void
		incIdx(int);

		int
		getIdx(void) const;

		void
		fillBuffer(char *raw_buffer);

		std::string &
		getBuffer(void);

		static bool
		isEndLine(std::string &line, int index);

		void
		clearBuffer(int);
};

#endif
