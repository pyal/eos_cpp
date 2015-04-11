#include <lib\precompiled\Ref.h>

//
// exenv.cc
//

//#include <stdlib.h>
//#include <iostream.h>

//#include <scconfig.h>
#include "exenv.h"
//#include <string.h>
//#include <stdarg.h>
//#include <stdio.h>


//using namespace std;

int ExEnv::initialized_ = 0;
unsigned long ExEnv::mem_ = 0;
int ExEnv::nproc_ = 0;
int *ExEnv::argc_ = 0;
char ***ExEnv::argv_ = 0;
char ExEnv::hostname_[256] = { '\0' };
char ExEnv::username_[9] = { '\0' };
ostream *ExEnv::out_ = 0;
FilterOut *ExEnv::fout_ = 0;

#include "class_sav.h"

#ifdef BinaryOutput
FilterBinOut fcout("Console_Output",DataSource::Console);
#else
FilterTextOut fcout("Console_Output",DataSource::Console);
#endif
//FilterTextIn fcin("Console_Input",DataSource::Console);

FilterOut &ExEnv::fout() { if (!fout_) set_fout(&fcout);return *fout_; }

const char *
ExEnv::program_name()
{
  if (argc_ == 0 || *argc_ == 0) return 0;
  char *start = strrchr((*argv_)[0],'/');
  if (!start) start = (*argv_)[0];
  else start++;
  return start;
}

void
ExEnv::init(int &argcref, char **&argvref)
{
  argc_ = &argcref;
  argv_ = &argvref;

#ifdef HAVE_GETHOSTNAME
  gethostname(hostname_, 256);
#else
  strcpy(hostname_, "UNKNOWN");
#endif

  memset(username_,0,9);
#if defined(HAVE_GETPWUID) && defined(HAVE_GETEUID)
  struct passwd *pw = getpwuid(geteuid());
  if (pw && pw->pw_name) {
      strncpy(username_, pw->pw_name, 9);
      username_[8] = 0;
    }
  else {
      strcpy(username_,"UNKNOWN");
    }
#else
  strcpy(username_,"UNKNOWN");
#endif

  initialized_ = 1;

}

scprintf::scprintf(const char *fmt, ...)
{
  va_list args;
  
  va_start(args, fmt);

  str[0] = '\0';
  
  // hopefully this won't overflow
  if (fmt && fmt[0]!='\0') {
    if (vsprintf(str, fmt, args) > 1023) {
      ExEnv::err() << indent << "scprintf overflow\n";
      abort();
    }
  }

  va_end(args);
}

ostream&
operator<<(ostream& o, const scprintf& s)
{
  o << s.str ;//<< flush;
  return o;
}

ios&
indent(ios&o)
{
  for (int i=0; i<1; i++) o.rdbuf()->sputc(' ');
  return o;
}

ios&
node0(ios& o)
{
//  if (!ready_) init();
  
//  if (!debug_ && node_to_print_ >= 0
//      && parallel_ && node_to_print_ != me_)
//    return nullstream_;

  return o;
}

// Local Variables:
// mode: c++
// c-file-style: "CLJ"
// End:
