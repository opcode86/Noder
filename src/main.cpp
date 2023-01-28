//                               ;                                 
//                        :      ED.                               
//    L.                 t#,     E#Wi                 ,;           
//    EW:        ,ft    ;##W.    E###G.             f#i j.         
//    E##;       t#E   :#L:WE    E#fD#W;          .E#t  EW,        
//    E###t      t#E  .KG  ,#D   E#t t##L        i#W,   E##j       
//    E#fE#f     t#E  EE    ;#f  E#t  .E#K,     L#D.    E###D.     
//    E#t D#G    t#E f#.     t#i E#t    j##f  :K#Wfff;  E#jG#W;    
//    E#t  f#E.  t#E :#G     GK  E#t    :E#K: i##WLLLLt E#t t##f   
//    E#t   t#K: t#E  ;#L   LW.  E#t   t##L    .E#L     E#t  :K#E: 
//    E#t    ;#W,t#E   t#f f#:   E#t .D#W;       f#E:   E#KDDDD###i
//    E#t     :K#D#E    f#D#;    E#tiW#G.         ,WW;  E#f,t#Wi,,,
//    E#t      .E##E     G#t     E#K##i            .D#; E#t  ;#W:  
//    ..         G#E      t      E##D.               tt DWi   ,KK: 
//                fE             E#t                               
//                 ,             L:                                
//                                                                                                       
// Author: koiradog
// Website: https://github.com/koiradog

#include <time.h>
#include "config.h"

#include "noder.h"


int main()
{
	ShowWindow(GetConsoleWindow(), config::hideself ? SW_HIDE : SW_SHOW);

	srand(time(0));

	Noder noder;

	return 0;
}