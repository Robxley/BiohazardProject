#ifndef _BHD_LOGGER_H
#define _BHD_LOGGER_H
#pragma once

#define BHD_USE_STDOUT_LOG

#ifdef _DEBUG
#define BHD_VERBOSE		//Use to enable the logger
#endif

#ifndef BHD_VERBOSE

#define BHD_LOG(msg)		
#define BHD_LOG_ERROR(msg)
#define BHD_LOG_WARNING(msg)

#else

#if defined(BHD_USE_STDOUT_LOG)
//#define BHD_USE_STDOUT_LOG
#elif defined(__ANDROID__) || defined(__IPHONEOS__) && defined(_SDL_H)
	#define BHD_USE_SDL_LOG
#else
	#define BHD_USE_FILE_LOG
	#ifndef BHD_LOG_FILE
	#define BHD_LOG_FILE "Biohazard.log"	//default log file
	#endif
#endif



#ifdef BHD_USE_SDL_LOG

#include <sstream>
#define BHD_LOG(msg)			{std::ostringstream a; a <<msg<<std::endl ; SDL_Log(a.str().c_str());}
#define BHD_LOG_ERROR(msg)		{std::ostringstream a; a <<"<ERROR> --"<<msg<<" -- FUNC <"<<__FUNCTION__<<"> -- FILE <"<<__FILE__<<"> -- LINE <"<<__LINE__<<">"<<std::endl ; SDL_Log(a.str().c_str());}
#define BHD_LOG_WARNING(msg)	{std::ostringstream a; a <<"<WARNING> --"<<msg<<" -- FUNC <"<<__FUNCTION__<<"> -- FILE <"<<__FILE__<<"> -- LINE <"<<__LINE__<<">"<<std::endl ; SDL_Log(a.str().c_str());}

#elif defined(BHD_USE_STDOUT_LOG)

#include<iostream>
#define BHD_LOG(msg)			{std::cout<<msg<<std::endl;}
#define BHD_LOG_ERROR(msg)		{std::cout<<"<ERROR> --"<<msg<<" -- FUNC <"<<__FUNCTION__<<"> -- FILE <"<<__FILE__<<"> -- LINE <"<<__LINE__<<">"<<std::endl ;}
#define BHD_LOG_WARNING(msg)	{std::cout<<"<WARNING> --"<<msg<<" -- FUNC <"<<__FUNCTION__<<"> -- FILE <"<<__FILE__<<"> -- LINE <"<<__LINE__<<">"<<std::endl ;}



#elif defined(BHD_USE_FILE_LOG)
	
#define BHD_LOG_INSTANCE(file)	{BHD::Logger::instance().getFileLogger(file);}
#define BHD_LOG(msg)			{BHD::Logger::instance().getFileLogger()<<msg<<std::endl;}
#define BHD_LOG_ERROR(msg)		{BHD::Logger::instance().getFileLogger()		<<"<ERROR> --"<<msg<<" -- FUNC <"<<__FUNCTION__<<"> -- FILE <"<<__FILE__<<"> -- LINE <"<<__LINE__<<">"<<std::endl;}
#define BHD_LOG_WARNING(msg)	{BHD::Logger::instance().getFileLogger()	<<"<WARNING> --"<<msg<<" -- FUNC <"<<__FUNCTION__<<"> -- FILE <"<<__FILE__<<"> -- LINE <"<<__LINE__<<">"<<std::endl;}


#include <fstream>
#include <iomanip>
#include <sstream>


namespace BHD
{
	/*!
	\author 	Robxley (A.CAILLY)
	\version	0.1
	\date		08/12/2014
	\~english
	\brief		Logger Class to log what you want in a speficique file. (See macro BH3D_LOGGER_FILE "BH3Dlogger.log").
	\remark		It's a singleton. See macros BH3D_LOGGER(msg), BH3D_LOGGERERROR(msg) and BH3D_LOGGERWARNING(msg).
	\~french
	\brief		Classer logger. Pour écrire ce que tu veux dans un fichier spécifique (voir macro BH3D_LOGGER_FILE "BH3Dlogger.log")
	\remark		C'est un singleton. Voir les macros BH3D_LOGGER(msg), BH3D_LOGGERERROR(msg) and BH3D_LOGGERWARNING(msg).
	*/
	class Logger
	{
		private:
			std::ofstream      logFile;
			/**
			*\~english
			*\brief		Private Constructor - Creates the file define by the macro BH3D_LOGGER_FILE.
			*\~french
			*\brief		Private Constructor - Créée le fichier défini par la macro BH3D_LOGGER_FILE.
			*/
			Logger(const char *filename);

			/**
			*\~english
			*\brief		Private Constructor - Closes the file.
			*\~french
			*\brief		Private Constructor - Ferme le fichier.
			*/
			~Logger();

		public:

			/**
			*\~english
			*\brief		Function to access to the logger.
			*\return	return a reference on logger.
			*\~french
			*\brief		Utiliser cette fonction pour avoir accès au logger.
			*\return	retourne une reference sur le logger.
			*/
			static Logger &instance(const char *filename = BHD_LOG_FILE);

			/**
			*\~english
			*\brief		gets the logger ofstream.
			*\return	return a reference on the used ofstream.
			*\~french
			*\brief		Pour obtenir le "ofstream" du logger.
			*\return	retourne une reference sur le ofstream utilisé par le logger.
			*/
			std::ofstream& getFileLogger();

	};

}
#endif

#endif	//BHD_VERBOSE
#endif	//_BHD_LOGGER_H

