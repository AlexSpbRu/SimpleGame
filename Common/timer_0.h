#ifdef _MSC_VER
void _cdecl IO_TRACE(const char* lpszFormat, ...);

typedef unsigned __int64 UINT64;
extern "C" UINT64 __rdtsc();
#pragma intrinsic(__rdtsc)



//#include "base/basictypes.h"   // make sure we get the def for int64
//#include "base/arm_instruction_set_select.h"
// base/sysinfo.h is really big and we don't want to include it unless
// it is necessary.
#if defined(__arm__) || defined(__mips__)
# include "base/sysinfo.h"
#endif
#if defined(__MACH__) && defined(__APPLE__)
# include <mach/mach_time.h>
#endif
// For MSVC, we want to use '_asm rdtsc' when possible (since it works
// with even ancient MSVC compilers), and when not possible the
// __rdtsc intrinsic, declared in <intrin.h>.  Unfortunately, in some
// environments, <windows.h> and <intrin.h> have conflicting
// declarations of some other intrinsics, breaking compilation.
// Therefore, we simply declare __rdtsc ourselves. See also
// http://connect.microsoft.com/VisualStudio/feedback/details/262047
#if defined(_MSC_VER) && !defined(_M_IX86)
extern "C" UINT64 __rdtsc();
#pragma intrinsic(__rdtsc)
#endif
#if defined(ARMV3) || defined(__mips__)
#include <sys/time.h>
#endif

// NOTE: only i386 and x86_64 have been well tested.
// PPC, sparc, alpha, and ia64 are based on
//    http://peter.kuscsik.com/wordpress/?p=14
// with modifications by m3b.  See also
//    https://setisvn.ssl.berkeley.edu/svn/lib/fftw-3.0.1/kernel/cycle.h
struct CycleClock {
	// This should return the number of cycles since power-on.  Thread-safe.
	static inline __int64 Now() {
#if defined(__MACH__) && defined(__APPLE__)
		// this goes at the top because we need ALL Macs, regardless of
		// architecture, to return the number of "mach time units" that
		// have passed since startup.  See sysinfo.cc where
		// InitializeSystemInfo() sets the supposed cpu clock frequency of
		// macs to the number of mach time units per second, not actual
		// CPU clock frequency (which can change in the face of CPU
		// frequency scaling).  Also note that when the Mac sleeps, this
		// counter pauses; it does not continue counting, nor does it
		// reset to zero.
		return mach_absolute_time();
#elif defined(__i386__)
		int64 ret;
		__asm__ volatile ("rdtsc" : "=A" (ret) );
		return ret;
#elif defined(__x86_64__) || defined(__amd64__)
		uint64 low, high;
		__asm__ volatile ("rdtsc" : "=a" (low), "=d" (high));
		return (high << 32) | low;
#elif defined(__powerpc__) || defined(__ppc__)
		// This returns a time-base, which is not always precisely a cycle-count.
		int64 tbl, tbu0, tbu1;
		asm("mftbu %0" : "=r" (tbu0));
		asm("mftb  %0" : "=r" (tbl));
		asm("mftbu %0" : "=r" (tbu1));
		tbl &= -static_cast<int64>(tbu0 == tbu1);
		// high 32 bits in tbu1; low 32 bits in tbl  (tbu0 is garbage)
		return (tbu1 << 32) | tbl;
#elif defined(__sparc__)
		int64 tick;
		asm(".byte 0x83, 0x41, 0x00, 0x00");
		asm("mov   %%g1, %0" : "=r" (tick));
		return tick;
#elif defined(__ia64__)
		int64 itc;
		asm("mov %0 = ar.itc" : "=r" (itc));
		return itc;
#elif defined(_MSC_VER) && defined(_M_IX86)
		// Older MSVC compilers (like 7.x) don't seem to support the
		// __rdtsc intrinsic properly, so I prefer to use _asm instead
		// when I know it will work.  Otherwise, I'll use __rdtsc and hope
		// the code is being compiled with a non-ancient compiler.
		_asm rdtsc
#elif defined(_MSC_VER)
		return __rdtsc();
#elif defined(ARMV3)
#if defined(ARMV6)  // V6 is the earliest arch that has a standard cyclecount
		uint32 pmccntr;
		uint32 pmuseren;
		uint32 pmcntenset;
		// Read the user mode perf monitor counter access permissions.
		asm volatile ("mrc p15, 0, %0, c9, c14, 0" : "=r" (pmuseren));
		if (pmuseren & 1) {  // Allows reading perfmon counters for user mode code.
			asm volatile ("mrc p15, 0, %0, c9, c12, 1" : "=r" (pmcntenset));
			if (pmcntenset & 0x80000000ul) {  // Is it counting?
				asm volatile ("mrc p15, 0, %0, c9, c13, 0" : "=r" (pmccntr));
				// The counter is set up to count every 64th cycle
				return static_cast<int64>(pmccntr) * 64;  // Should optimize to << 6
			}
		}
#endif
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return static_cast<int64>((tv.tv_sec + tv.tv_usec * 0.000001)
			* CyclesPerSecond());
#elif defined(__mips__)
		// mips apparently only allows rdtsc for superusers, so we fall
		// back to gettimeofday.  It's possible clock_gettime would be better.
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return static_cast<int64>((tv.tv_sec + tv.tv_usec * 0.000001)
			* CyclesPerSecond());
#else
		// The soft failover to a generic implementation is automatic only for ARM.
		// For other platforms the developer is expected to make an attempt to create
		// a fast implementation and use generic version if nothing better is available.
#error You need to define CycleTimer for your O/S and CPU
#endif
	}
};




inline UINT64 get_cyrcle_0(void){return __rdtsc();}

#pragma intrinsic(__debugbreak)
extern "C" void __debugbreak();

#endif

extern bool g_bTrace;

#ifndef _MSC_VER
#include <time.h>
typedef unsigned long int INT64;

inline INT64 get_cyrcle_0(void)
{ 
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return  tv.tv_usec ;
}
#endif




inline double perfomance_0()
{	
#ifndef _MSC_VER
	return 1e-9;
#endif
#ifdef _MSC_VER
	UINT64 _s=get_cyrcle_0();
	Sleep(50);
	UINT64 _e=get_cyrcle_0();
	return 0.05/double(_e-_s);
#endif

}
typedef  unsigned __int64 UINT64;
inline double time_0()
{
	static const double ___perfomance =  perfomance_0();
	static const UINT64 ___start = get_cyrcle_0();
	return ___perfomance * ( (INT64)(get_cyrcle_0()) -___start );
}

struct STimeCountInf
{
	UINT64 T;
	int C;	
	UINT64 C1;
};

inline bool pless(const  std::pair<const char*, STimeCountInf >& _1,const std::pair<const char*, STimeCountInf >& _2)
{
	return _1.second.T<_2.second.T;
}
//#define USE_TIMER
# ifdef _MSC_VER
//#define USE_TIMER
#endif 
# ifndef _MSC_VER
extern void* g_TimeMap;
#endif

template <class T>
class CEnumeratorTimer
{
	
public:
	typedef  std::unordered_map<const  char* , STimeCountInf > TimeMap;
	typedef  typename TimeMap::const_iterator I;	

# ifdef _MSC_VER
	static void* g_TimeMap;
#endif
	
	bool recursive;
	static void PrintTimeClear()
	{
#ifdef USE_TIMER
		PrintTime();
		((TimeMap*)g_TimeMap)->clear();
#endif
	}
	static void PrintTimeStart()
	{
#ifdef USE_TIMER
		g_TimeMap = new TimeMap;
#endif
	}
	static void PrintTime()
	{
#ifdef USE_TIMER		
		static const double perfomance =  1e6*perfomance_0();

		std::vector<   std::pair< const  char*, STimeCountInf >  > v;
		for( I i=((TimeMap*)g_TimeMap)->begin(); i!=((TimeMap*)g_TimeMap)->end();++i)
			v.push_back( std::pair< const char*, STimeCountInf >((*i).first,(*i).second ) );

		std::sort( v.begin(), v.end(), pless );

		for(std::vector<  std::pair<const  char*, STimeCountInf >  >::iterator i=v.begin();i!=v.end();++i)
		{
			const char* p=(*i).first;
			std::string n(p);
			for(size_t ii=n.size(); ii<40;ii++)n+=' ';
			char c1m=' ';
			INT64 c1=(*i).second.C1;
			int cc1;
			if(c1>1024*1024*1024)
				cc1=(int)(c1/(1000*1000)),c1m='M';
			else if(c1>1000*1000)
				cc1=(int)(c1/(1000)),c1m='K';
			else
				cc1=(int)c1;
			double T_=perfomance*((*i).second.T);
			const char* mks="mkc";const char* ms="mc";const char* s="sec";const char* H="hour";
			const char* sss=mks;
			if(T_>3600.0*1e6)
				T_/=3600.0*1e6,sss=H;
			else if(T_>1e6)
				T_/=1e6,sss=s;
			else if(T_>1e3)
				T_/=1e3,sss=ms;
		
			//IO_TRACE( "%s\n",n.c_str() );
			//IO_TRACE("time=%.2f %s\n",n.c_str(),(float)T_,sss );
			
			IO_TRACE("%s time=%.2f %s count=%d  c1=%d %c\n",n.c_str(),(float)T_,sss,(*i).second.C ,cc1, c1m);		
		}		 
#endif	
	}
	CEnumeratorTimer()
	{
	}

	~CEnumeratorTimer()
	{
#ifdef USE_TIMER
		T* p=static_cast<T*>(this);
		const char * k=p->GetStringKey() ;
		STimeCountInf& ti=(*(TimeMap*)g_TimeMap)[ k];
		ti.T += p->GetTime();
		ti.C++;
		ti.C1+=p->GetC1();
#endif
	}

};


class timer_0 : public CEnumeratorTimer<timer_0>
{
const char * stringkey;
INT64 start;
INT64 C1;
public:
#ifdef USE_TIMER
	timer_0(const char * p):stringkey(p),start(get_cyrcle_0()),C1(0){}
	timer_0(const char * p,INT64 c1):stringkey(p),start(get_cyrcle_0()),C1(c1){}
	void Add(UINT c){ C1+=c;}
	~timer_0()
	{	
		start=(get_cyrcle_0() - start);
		// IO_TRACE("%s time=%f mkc\n",stringkey,start ); 
    }
#else
	timer_0(const char * p){}
	timer_0(const char * p,INT64 c1){}
	void Add(UINT c){}
	~timer_0(){}
#endif
	const char * GetStringKey()  { return stringkey;}
	INT64 GetTime() { return start ;}
	INT64 GetC1() { return C1 ;}
};

# ifdef _MSC_VER
void*  CEnumeratorTimer< timer_0>::g_TimeMap;

#endif
 



