﻿#pragma once

#ifndef __FINAL__
#define __FINAL__

// Check CSpaceEngine has installed
#ifdef __has_include
#if !__has_include(<CSE/CSE.h>)
#error "Compile or install CSpaceEngine to continue."
#endif
#endif

#include <vector>
#include <CSE/SCStream.h>
#include <CSE/Object.h>

#include "html/gbuffer_html.h"

enum OutputFormat
{
	HTML,
	MD
};

extern bool Astrobiology;
extern OutputFormat OFormat;
extern std::string Final;
using ObjectBuffer = std::vector<cse::Object>;
extern ObjectBuffer System;
extern std::map<std::string, std::string> LocStrings;
extern std::string CSSPath;
extern enum LinkCSS LCSS;
extern int outencoding;
extern std::string OutputFileName;

void Transcode(std::string& arg, int encoding);

#endif