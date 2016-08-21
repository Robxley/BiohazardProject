/*
 * The MIT License
 *
 * Copyright 2014 Robxley.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


/*!
\file Assertion.hpp
\author Robxley (A.CAILLY)
\version 0.1
\date 09/12/2014
\~english
\brief Biohazard3d assertion macro
\details  Describe the Assertion routines and macros
\~french
\brief Macro pour les assertions
\details  Déscriptions des routines et macros utilisées par Biohazard3d
*/

#ifndef _BHD_ASSERTION_H
#define _BHD_ASSERTION_H
#pragma once

#include <cassert>

/**
*\~english
*\brief		Biohazard assertion macro
*\param[in]	Expression to be tested
*\~french
*\brief		Macro assertion utilisée par biohazard
*\param[in]	Expression à tester
*/
#define BHD_ASSERT(w)  {if(!(w)){ BHD_LOG_ERROR((#w));}} assert(w);

#ifdef _DEBUG
#define BHD_ASSERT_LOG(w,msg)	{BHD_ASSERT(w); if(!(w)){ BHD_LOG_ERROR(msg) ;}}
#define BHD_ASSERT_LOG_R(w,msg,r) {BHD_ASSERT(w); if(!(w)){ BHD_LOG_ERROR(msg) ; return r;}}
#else
#define BHD_ASSERT_LOG(w,msg)	
#define BHD_ASSERT_LOG_R(w,msg,r) 
#endif

#include "Logger.hpp"
#include <sstream>
#define BHD_THROW_WITH_LOG(msg) {BHD_LOG_ERROR(msg); std::ostringstream a; a << "msg"; throw std::runtime_error(a.str().c_str());}

#endif //_BHD_ASSERTION_H

