#ifndef __FUSION_SYNTAX_HIGHLIGHT_TOKENS__
#define __FUSION_SYNTAX_HIGHLIGHT_TOKENS__

#include <vector>
#include "../../../Utilites/EditorUtilites.h"

//Ensure that the order of syntax must be same throughout all tokens! (or at least contain the same syntax to properly work)
const std::map<std::string,SyntaxHighlightLayout> HighlightToken = {
	{ "or",		{glm::vec4(155.0f, 20.0f, 20.0f, 0.0f),	SYNTAX_FUNCTION} },
	{ "and",	{glm::vec4(155.0f, 20.0f, 20.0f, 0.0f),	SYNTAX_FUNCTION} },
	{ "not",	{glm::vec4(155.0f, 20.0f, 20.0f, 0.0f),	SYNTAX_FUNCTION} },
	{ "xor",	{glm::vec4(155.0f, 20.0f, 20.0f, 0.0f),	SYNTAX_FUNCTION} },

	{ "||",		{glm::vec4(155.0f, 20.0f, 20.0f, 0.0f),	SYNTAX_BITWISE_FUNCTION} },
	{ "<<",		{glm::vec4(155.0f, 20.0f, 20.0f, 0.0f),	SYNTAX_BITWISE_FUNCTION} },
	{ ">>",		{glm::vec4(155.0f, 20.0f, 20.0f, 0.0f),	SYNTAX_BITWISE_FUNCTION} },
	{ "&&",		{glm::vec4(155.0f, 20.0f, 20.0f, 0.0f),	SYNTAX_BITWISE_FUNCTION} },
	{ "&",		{glm::vec4(155.0f, 20.0f, 20.0f, 0.0f),	SYNTAX_BITWISE_FUNCTION} },
	{ "~",		{glm::vec4(155.0f, 20.0f, 20.0f, 0.0f),	SYNTAX_BITWISE_FUNCTION} },

	{ "int",	{glm::vec4(152.0f, 3.0f, 252.0f, 0.0f),	SYNTAX_DATATYPE} },
	{ "cint",	{glm::vec4(152.0f, 3.0f, 252.0f, 0.0f),	SYNTAX_DATATYPE} },
	{ "float",  {glm::vec4(152.0f, 3.0f, 252.0f, 0.0f),	SYNTAX_DATATYPE} },
	{ "cfloat", {glm::vec4(152.0f, 3.0f, 252.0f, 0.0f),	SYNTAX_DATATYPE} },
	{ "bool",	{glm::vec4(152.0f, 3.0f, 252.0f, 0.0f),	SYNTAX_DATATYPE} },
	{ "cbool",  {glm::vec4(152.0f, 3.0f, 252.0f, 0.0f),	SYNTAX_DATATYPE} },

	{ "vec2",	{glm::vec4(152.0f, 3.0f, 252.0f, 0.0f),	SYNTAX_DATATYPE} },
	{ "cvec2",	{glm::vec4(152.0f, 3.0f, 252.0f, 0.0f),	SYNTAX_DATATYPE} },
	{ "vec3",	{glm::vec4(152.0f, 3.0f, 252.0f, 0.0f),	SYNTAX_DATATYPE} },
	{ "cvec3",	{glm::vec4(152.0f, 3.0f, 252.0f, 0.0f),	SYNTAX_DATATYPE} },
	{ "vec4",	{glm::vec4(152.0f, 3.0f, 252.0f, 0.0f),	SYNTAX_DATATYPE} },
	{ "cvec4",	{glm::vec4(152.0f, 3.0f, 252.0f, 0.0f),	SYNTAX_DATATYPE} },

	{ "noreturn", {glm::vec4(252.0f, 103.0f, 3.0f, 0.0f), SYNTAX_DATATYPE} }
};


#endif
