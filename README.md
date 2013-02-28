oclinclude
==========

Parses a file or string of text and tries to replace the include calls to the content of the included file. The lack of an include directive in OpenCL is thus lifted.

To use, simply include the header.
The two functions to use are:
static const char *fparseIncludes(const char *filename)
static const char *parseIncludes(const char *text)

Where fparseIncludes takes the name of a file and parseIncludes takes a bunch of text (the content of a previously opened file for example).
