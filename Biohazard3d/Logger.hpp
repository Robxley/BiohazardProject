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
#include <iomanip>
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

		//field width -> std::setw(n*step);
		int n_field ;	
		int step_field;	
		/**
		*\~english
		*\brief		Private Constructor - Creates the file define by the macro BH3D_LOGGER_FILE.
		*\~french
		*\brief		Private Constructor - Créée le fichier défini par la macro BH3D_LOGGER_FILE.
		*/
		BasicLogger(void *data = nullptr) : log(init(data)), n_field(0), step_field(5)
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

		T& getLogger() { 
			log << std::setw(n_field*step_field)<<"";
			return log; 
		}

		void setStepField(int step)  {
			if (step > 0) step_field = step;
		}

		void resetPushPop() { 
			n_field = 0;
		}

		void pushField() { 
			n_field++; 
			if (n_field < 0) n_field = 0; 
		}
		void popField() { 
			n_field--; 
			if (n_field < 0) n_field = 0; 
		}

		auto Field() { return std::setw(n_field*step_field); }
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
#define BHD_LOG_PUSH			{bhd::Logger::instance().pushField();}
#define BHD_LOG_POP				{bhd::Logger::instance().popField();}
#define BHD_LOG_RESET_PP		{bhd::Logger::instance().resetPushPop();}
#define BHD_LOG_STEP_PP(n)		{bhd::Logger::instance().setStepField(n);}
#define BHD_LOG(msg)			{bhd::Logger::instance().getLogger()<<msg<<std::endl;}
#define BHD_LOG_ERROR(msg)		BHD_LOG("<ERROR> --"<<msg<<" -- FUNC <"<<__FUNCTION__<<"> -- FILE <"<<__FILE__<<"> -- LINE <"<<__LINE__<<">")
#define BHD_LOG_WARNING(msg)	BHD_LOG("<WARNING> --"<<msg<<" -- FUNC <"<<__FUNCTION__<<"> -- FILE <"<<__FILE__<<"> -- LINE <"<<__LINE__<<">")
#define BHD_LOG_LIST(title,list){BHD_LOG(title); BHD_LOG_PUSH; for(const auto & one:list){BHD_LOG(one)}; BHD_LOG_POP;}	

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

