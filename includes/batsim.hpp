/**
 * @file batsim.hpp
 * @brief Header for command line interface
 * 
 * Defines the command that the CLI will accept
 * @author Dipta Pandit
 * @date 3/12/2014
 * @see batsim.cpp
 */
#ifndef  BATSIM_H
#define  BATSIM_H


#define GETCVOL		00 ///<get cvoltage
#define GETLOAD		01 ///<get load
#define GETSCUR		02 ///<get scurrent
#define GETVOUT		03 ///<get vout
#define GETIOUT		04 ///<get iout
#define GETSTAT		05 ///<get status
#define GETRUNT		06 ///<get runtime
#define GETSWCH		15 ///<get sstate

#define SETINTV		114 ///<set initv <v1> <v2> <v3>
#define SETLOAD		101 ///<set load <v1>
#define SETSRES		107 ///<set sres <v1> <v2> <V3>

#define SETADVSH	210 ///<setadv shift <cell> <shift1> <shift2>
#define SETADVDP	211 ///<setadv drop <cell> <drop1> <drop2>
#define SETADVCUT	212 ///<setadv cutoff <v1>
#define SETADVCAP	213 ///<setadv capacity <v1> <v2> <v2>

#define SIMSTART	308 ///<sim start
#define SIMSTOP		309 ///<sim stop

#define HELP		400 ///<help
#define EXIT		500 ///<exit


#endif //BATSIM_H
