/****************************************************************************
|*  SimpleXMLNode.h
|*
|*  Simple XML Reader. Extension of Daniel Howards bxmlnode.cpp (see below)
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li - Applied Geometry Group ETH Zürich 2006
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch
\***********************************************************/

/*
Copyright (c) 2003 Daniel W. Howard

Permission is hereby granted, free of charge, to any
person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the
Software without restriction, including without
limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice
shall be included in all copies or substantial portions of
the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. IN
NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
FOR ANY SPECIAL, INDIRECT, INCIDENTAL, OR CONSEQUENTIAL
DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <string.h>
#include <stdio.h>

/* basicxmlnode: simple xml node memory representation */
struct basicxmlnode
{
  char * tag; /* xml tag. always non-NULL */
  char * text; /* body text + all whitespace, always non-NULL */
  char * * attrs; /* atribute names. array of strings, NULL marks end */
  char * * values; /* attribute values. array of strings, NULL marks end */
  struct basicxmlnode * * children; /* array of pointers to children basicxmlnodes. NULL marks end */
  int * childreni; /* children positions in text. if for example childreni[0] is 3 and text is "abcdefg"
				   the xml-node of the first child was placed after "abc" and before "defg" */
};

/* searches for the first child node of 'node' with the tag-name 'name'. returns NULL if search insucessful */
struct basicxmlnode * getchildnodebyname( struct basicxmlnode * node, char * name );

/* searches for the value of the attribute with the given name. returns NULL if search insucessful */
char * getattributevaluebyname( struct basicxmlnode * node, char * name );

/* deletebasicxmlnode: frees all memory for xml tree */
void deletebasicxmlnode( struct basicxmlnode * node );

/* readbasicxmlnode: reads simple XML file */
struct basicxmlnode * readbasicxmlnode( FILE * fpi );

/* printbasicxmlnode: prints to console */
void printbasicxmlnode( struct basicxmlnode * node );

/* printbasicxmlnodetagnames: visits each node once */
void printbasicxmlnodetagnames( struct basicxmlnode * node );
