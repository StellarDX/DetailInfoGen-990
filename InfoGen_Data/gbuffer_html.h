﻿#pragma once

#ifndef __GBUF_HTML__
#define __GBUF_HTML__

#include <xstring>
#include "final.h"

class _Html_Tags // Tags
{
public:
	static constexpr auto _DOCTYPE     = "<!DOCTYPE html>"; // HTML5 type
	static constexpr auto _html_begin  = "<html>";
	static constexpr auto _html_end    = "</html>";
	static constexpr auto _head_begin  = "<head>";
	static constexpr auto _head_end    = "</head>";
	static constexpr auto _link        = "<link rel=\"stylesheet\" type=\"text/css\" href=\"{}\">"; // FMT string
	static constexpr auto _style_begin = "<style type=\"text/css\">";
	static constexpr auto _style_end   = "</style>";
	static constexpr auto _meta        = "<meta charset=\"{}\">"; // HTML only support utf-8 encoding.
	static constexpr auto _title_begin = "<title>";
	static constexpr auto _title_end   = "</title>";
	static constexpr auto _body_begin  = "<body>";
	static constexpr auto _body_end    = "</body>";
	static constexpr auto _div_begin   = "<div class=\"{}\">";
	static constexpr auto _div_end     = "</div>";
	static constexpr auto _html_endl   = "<br>";
	static constexpr auto _span_begin  = "<span class = \"topmost\">";
	static constexpr auto _span_end    = "</span>";
	static constexpr auto _h1_begin    = "<h1 class = \"topmost\">";
	static constexpr auto _h1_end      = "</h1>";
	static constexpr auto _h2_begin    = "<h2 class = \"topmost\">";
	static constexpr auto _h2_end      = "</h2>";
	static constexpr auto _hx_begin    = "<h{} class = \"topmost\">";
	static constexpr auto _hx_end      = "</h{}>";
	static constexpr auto _table_begin = "<table class = \"general\" border=\"1\">";
	static constexpr auto _table_end   = "</table>";
	static constexpr auto _html_line   = "<hr>";
	static constexpr auto _para_begin  = "<p>";
	static constexpr auto _para_end    = "</p>";
};

extern enum CopyDefaultMethod
{
	Asking,
	Skip,
	Replace
}DefaultCCpyMethod;

extern std::string HTMLhead;
extern std::string HTMLcontent;
extern std::string HTMLMenu;

std::string MakeHTMLHead(std::string Title, int Charset, std::string CSSPath = "", LinkCSS Copy = Static);

#endif