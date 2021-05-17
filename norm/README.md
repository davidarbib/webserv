###Norm

##general

templates is .hpp
class declarations is .hpp
class implementation is .cpp

empty line end of file
tab indentation 4-space ofc

comparisons and math operators are blank-isolated

no parentheses if not necessary in return

#class declaration

template line above ClassName line
ClassName
permission order: private / protected / public 

member declaration : `_member_name` (snake_case)
this->member

##methods and variables qualifiers

use const by default
const is placed after the constant object name
const is place between type and name

ref and pointer signs is placed before name and attached

explicit "void" if void arguments


in method declaration or implementation:
first line for type specification
second for function name and arguments
third if throws specification



casts must be explicit and C++ style


##naming

variable name is snake_case
methodname is camelCase

the function and variables names must be well chosen
function roles and size:

##functions

no size requested but ask yourself how you can respect these points whether your
functions have more than 30 lines:

- the functions must have only **one** role
- try to define roles **precisely**
- try to not cumulate multiple levels of abstraction within one function

tip: try to write down in comment the operations sequence in pseudo code and
replace comment line by true code.

##comments

comments must not repeat the code
comments can specify inputs and outputs of your components
comments can serve to explain your implementation choices
one line comment is just // and the line must be dedicated for comment
`/**
  * multi line
  * comment
  */`

https://developer.lsst.io/cpp/api-docs.html

