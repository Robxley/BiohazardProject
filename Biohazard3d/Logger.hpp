#ifndef _BHD_LOGGER_H
#define _BHD_LOGGER_H
#pragma once

//#define BHD_USE_FILE_LOG

#ifdef _DEBUG
#define BHD_VERBOSE		//Use to enable the logger
#endif

#ifdef BHD_VERBOSE

#if defined(__ANDROID__) || defined(__IPHONEOS__) && defined(_SDL_H)
	#define BHD_USE_SDL_LOG
#elif defined(BHD_USE_FILE_LOG)
	#ifndef BHD_LOG_FILE
	#define BHD_LOG_FILE "Biohazard.log"	//default log file
	#endif
#else
	#define BHD_USE_STDOUT_LOG
#endif


#include "Assertion.hpp"
#include "Singleton.hpp"
namespace bhd
{
	/*!
	\author 	Robxley (A.CAILLY)
	\version	0.1
	\date		08/12/2014
	\~english
	\brief		Logger Class.
	\remark		It's a singleton.
	*/
	template <typename T, typename D = T>
	class BasicLogger
	{
	protected:
		T log;
		/**
		*\~english
		*\brief		Private Constructor - Creates the file define by the macro BH3D_LOGGER_FILE.
		*\~french
		*\brief		Private Constructor - Créée le fichier défini par la macro BH3D_LOGGER_FILE.
		*/
		BasicLogger(void *data = nullptr) : log(init(data))
		{ 		
			log << "  ================================"	<< std::endl;
			log << "   Biohazard - Logger - START "		<< std::endl;
			log << "  ================================"	<< std::endl << std::endl;
		}

		/**
		*\~english
		*\brief		Private Constructor - Closes the file.
		*\~french
		*\brief		Private Constructor - Ferme le fichier.
		*/
		~BasicLogger() {
			
			log << std::endl;
			log << "  ===============================" << std::endl;
			log << "   Biohazard - Logger - END " << std::endl;
			log << "  ===============================" << std::endl << std::endl;
		}

		/**
		*\~english
		*\brief		Initialisation.
		*\~french
		*\brief		Initialisation.
		*/
		D init(void *data) { return data; }

	public:

		T& getLogger() { return log; }

	};

}


#ifdef BHD_USE_SDL_LOG


#elif defined(BHD_USE_STDOUT_LOG)

#include <iostream>
#include <ostream>
#include <functional>
namespace bhd
{
	class Logger : public BasicLogger<std::ostream &> , public Singleton<Logger>
	{
		friend class Singleton<Logger>;
	protected:
		Logger(void *data){}
	};	
	//Specialisation
	template <>
	inline std::ostream& BasicLogger<std::ostream&>::init(void *data)
	{
		return std::cout;
	}
}

#elif defined(BHD_USE_FILE_LOG)

#include <fstream>
namespace bhd
{
	class Logger : public BasicLogger<std::ofstream, const char*>, public Singleton<Logger>
	{
		friend class Singleton<Logger>;
	protected:
		Logger(void *data) : BasicLogger(data) {}
	};

	//Specialisation
	template <>
	inline const char* BasicLogger<std::ofstream, const char*>::init(void *data)
	{
		const char * defaultfile = BHD_LOG_FILE;
		return data==nullptr ? defaultfile : (const char*)data;
	}
}

#endif


#define BHD_LOG_INSTANCE(data)	{bhd::Logger::instance((void*)data);}
#define BHD_LOG(msg)			{bhd::Logger::instance().getLogger()<<msg<<std::endl;}
#define BHD_LOG_ERROR(msg)		{bhd::Logger::instance().getLogger()<<"<ERROR> --"<<msg<<" -- FUNC <"<<__FUNCTION__<<"> -- FILE <"<<__FILE__<<"> -- LINE <"<<__LINE__<<">"<<std::endl;}
#define BHD_LOG_WARNING(msg)	{bhd::Logger::instance().getLogger()<<"<WARNING> --"<<msg<<" -- FUNC <"<<__FUNCTION__<<"> -- FILE <"<<__FILE__<<"> -- LINE <"<<__LINE__<<">"<<std::endl;}

#endif	//BHD_VERBOSE

#ifndef BHD_LOG_INSTANCE
#define BHD_LOG_INSTANCE(file)
#endif

#ifndef BHD_LOG
#define BHD_LOG(msg)	
#endif

#ifndef BHD_LOG_ERROR
#define BHD_LOG_ERROR(msg)	
#endif

#ifndef BHD_LOG_WARNING
#define BHD_LOG_WARNING(msg)	
#endif


#endif	//_BHD_LOGGER_H

