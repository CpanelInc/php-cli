# AC_COVERAGE                                                           

AN_MAKEVAR([LCOV], [AC_PROG_LCOV])                                      
AN_PROGRAM([lcov], [AC_PROG_LCOV])                                      
AC_DEFUN([AC_PROG_LCOV],                                                
[                                                                       
  AC_PATH_TOOL(LCOV, lcov, :)                                           
])                                                                      

AN_MAKEVAR([GENHTML], [AC_PROG_GENHTML])                                
AN_PROGRAM([genhtml], [AC_PROG_GENHTML])                                
AC_DEFUN([AC_PROG_GENHTML],                                             
[                                                                       
  AC_PATH_TOOL(GENHTML, genhtml, :)                                     
])                                                                      

AN_MAKEVAR([GCOV], [AC_PROG_GCOV])                                      
AN_PROGRAM([gcov], [AC_PROG_GCOV])                                      
AC_DEFUN([AC_PROG_GCOV],                                                
[                                                                       
  if test "$GCC" != "yes"; then                                         
    AC_MSG_ERROR([GCC is require for gcov])                             
  fi                                                                    
  AC_PATH_TOOL(GCOV, gcov, :)                                           
  COVERAGE_CFLAGS="--coverage -DDEBUG"                                  
  AC_SUBST(COVERAGE_CFLAGS)                                             
  COVERAGE_CXXFLAGS="--coverage -DDEBUG"                                
  AC_SUBST(COVERAGE_CXXFLAGS)                                           
  COVERAGE_LDFLAGS="--coverage -lgcov"                                  
  AC_SUBST(COVERAGE_LDFLAGS)                                            
  COVERAGE_OPTFLAGS="-O0"                                               
  AC_SUBST(COVERAGE_OPTFLAGS)                                           
])                                                                      

AC_DEFUN([AC_COVERAGE],                                                 
[                                                                       
  AC_PROG_GCOV                                                          
  AC_PROG_LCOV                                                          
  AC_PROG_GENHTML                                                       
])                                                                      

